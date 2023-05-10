// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterBase.h"

// Sets default values
AAICharacterBase::AAICharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthSystem>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->SetHealthToMaxHealth();
}

// Called every frame
void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AAICharacterBase::TakeDamage(float Amount)
{
	bool test= HealthComponent->TakeDamage(Amount);
	if (test )
	{
		OnDeath();
	}
	return test;
}

void AAICharacterBase::RecoverHealth(float Amount)
{
	HealthComponent->RecoverHealth(Amount);
}
