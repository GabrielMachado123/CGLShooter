#include "Shooter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


//////////////////////////////////////////////////////////////////////////
// AShooter

AShooter::AShooter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HealthComponent = CreateDefaultSubobject<UHealthSystem>(TEXT("Health Component"));
	StatComponent = CreateDefaultSubobject<UStatSystem>(TEXT("Stat Component"));
	StatusEffectComponent = CreateDefaultSubobject<UStatusEffectsComponent>(TEXT("Status Effect Component"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AShooter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		CachedWorld = World;
	}


	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	PopulateSkillArray();

	HealthComponent->SetMaxHealth(StatComponent->GetMaxHealth());
	HealthComponent->SetHealthToMaxHealth();
	StatusEffectComponent->SetStatsComponent(StatComponent);

	GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetMovementSpeed();
}

void AShooter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Controller != nullptr)
	{
		FRotator CharacterDirection = FRotator(Controller->GetControlRotation());
		CharacterDirection.Pitch = 0;

		GetCapsuleComponent()->SetWorldRotation(CharacterDirection);
	}

	if (GetMovementComponent()->IsMovingOnGround() && GroundedTime < JumpBuffer)
	{
		GroundedTime += DeltaSeconds;
	}

	if(bIsMovingBackwards && !bIsSlowApplied)
	{
		GetCharacterMovement()->MaxWalkSpeed *= .8f;
		bIsSlowApplied = true;
	}else if(!bIsMovingBackwards && bIsSlowApplied)
	{
		GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetMovementSpeed();
		bIsSlowApplied = false;
	}
}

bool AShooter::GetIsCasting()
{
	return bIsCasting;
}

void AShooter::SetIsCasting(bool Value)
{
	bIsCasting = Value;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShooter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
		                                   &AShooter::TestGround);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &AShooter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &AShooter::Look);

		//Basic Attack
		EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this,
		                                   &AShooter::UseBasicAttack);
		//First Ability
		EnhancedInputComponent->BindAction(FirstAbilityAction, ETriggerEvent::Triggered, this,
		                                   &AShooter::UseFirstAbility);
		//Second Ability
		EnhancedInputComponent->BindAction(SecondAbilityAction, ETriggerEvent::Triggered, this,
		                                   &AShooter::UseSecondAbility);
		//Third Ability
		EnhancedInputComponent->BindAction(ThirdAbilityAction, ETriggerEvent::Triggered, this,
		                                   &AShooter::UseThirdAbility);
	}
}

void AShooter::TakeDamage(float Amount)
{
	HealthComponent->TakeDamage(Amount);
}

void AShooter::RecoverHealth(float Amount)
{
	HealthComponent->RecoverHealth(Amount);
}

void AShooter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	CachedMovementVector = MovementVector;

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

	bIsMovingBackwards = (MovementVector.Y <= 0) ? true : false; 

}

void AShooter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShooter::TestGround()
{
	if (GroundedTime >= JumpBuffer)
	{
		ACharacter::Jump();
		GroundedTime = 0;
	}
}

void AShooter::PopulateSkillArray()
{
	for (int i = 0; i < Skills.Num(); ++i)
	{
		if (USkillBase* NewSkill = NewObject<USkillBase>(this, Skills[i]))
		{
			NewSkill->InitializeSkill(this, CachedWorld, 0);
			RuntimeSkills.Add(NewSkill);
		};
	}
}

FRotator AShooter::CalculateShootingAngle(const FVector InitialPoint,
                                          const float Range) const
{
	const FVector TraceStart = GetFollowCamera()->GetComponentLocation();
	const FVector TraceEnd = (UKismetMathLibrary::GetForwardVector(
		UGameplayStatics::GetPlayerCameraManager(CachedWorld, 0)->GetCameraRotation()) * Range) + TraceStart;

	FHitResult OutHit;
	const TArray<AActor*> ActorsToIgnore;

	if (UKismetSystemLibrary::LineTraceSingle(CachedWorld, TraceStart, TraceEnd,
	                                          ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore,
	                                          EDrawDebugTrace::Persistent, OutHit, true))
	{
		return UKismetMathLibrary::FindLookAtRotation(InitialPoint, OutHit.ImpactPoint);
	}
	return UKismetMathLibrary::FindLookAtRotation(InitialPoint, TraceEnd);
}

void AShooter::UseBasicAttack()
{
	// GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Red,TEXT("Shot"));
	if (RuntimeSkills.IsValidIndex(0) && !GetIsCasting() && RuntimeSkills[0]->bCanUse)
	{
		CachedMouseRotator = CalculateShootingAngle(ShootingPoint->GetComponentLocation(), AttackRange);
		RuntimeSkills[0]->CastSkill(AttackAnimations[0], StatComponent->GetAttackSpeedAsCooldown(),
		                            StatComponent->GetDamage());
	}
}

void AShooter::UseFirstAbility()
{
	CachedMouseRotator = CalculateShootingAngle(ShootingPoint->GetComponentLocation(), AttackRange);
	GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Blue,TEXT("First"));
}

void AShooter::UseSecondAbility()
{
	CachedMouseRotator = CalculateShootingAngle(ShootingPoint->GetComponentLocation(), AttackRange);
	GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Green,TEXT("Second"));
}

void AShooter::UseThirdAbility()
{
	CachedMouseRotator = CalculateShootingAngle(ShootingPoint->GetComponentLocation(), AttackRange);
	GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Black,TEXT("Third"));
}
