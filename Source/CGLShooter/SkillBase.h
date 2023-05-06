// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableCharacterSkillSlot.h"
#include "UsableSkill.h"
#include "SkillBase.generated.h"

/**
 * 
 */
class AShooter;
UCLASS(Blueprintable)
class CGLSHOOTER_API USkillBase : public UObject, public IUsableSkill
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable)
	virtual void InitializeSkill(AShooter* Playable, UWorld* World, int Team) override;
	UFUNCTION(BlueprintCallable)
	virtual void UseSkill() override;
	void StartCooldownTimer();
	virtual void CastSkill(UAnimMontage* AnimationToPlay, float CooldownModifier, float Damage) override;

	UFUNCTION()
	void ResetCooldown();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCast();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUse();
	UFUNCTION(BlueprintImplementableEvent)
	void OnInitialize();

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float AbilityCooldown;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float AbilityCooldownModifier;
	UFUNCTION(BlueprintCallable)
	void SetCooldown(float Cooldown);
	UFUNCTION()
	void SetCooldownModifier(float Amount) {AbilityCooldownModifier = Amount;}

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float AbilityCastTime;
	UFUNCTION(BlueprintCallable)
	void SetCastTime(float CastTime);

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float AbilitySpawnTime;
	UFUNCTION(BlueprintCallable)
	void SetSpawnTime(float SpawnTime);

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float AbilityDamage;
	float Power;
	UFUNCTION(BlueprintCallable)
	void SetAbilityDamage(float AbilityPower, float AddValue);

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float AbilityRange;
	UFUNCTION(BlueprintCallable)
	void SetAbilityRange(float Range);

	UPROPERTY(BlueprintReadWrite)
	bool bCanUse;

	UPROPERTY(BlueprintReadWrite)
	AShooter* OwnerCharacter;
	UPROPERTY(BlueprintReadWrite)
	UWorld* CachedWorld;
	IUsableCharacterSkillSlot* CachedCharacterInterface;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<AActor> CastEffectToSpawn;
	UPROPERTY(BlueprintReadWrite)
	FRotator AbilityRotation;

	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* AttackAnimation;

	UFUNCTION(BlueprintCallable)
	int GetTeamId() const { return TeamId; }

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnSkillActor(const FVector& SpawnPosition);
	UPROPERTY()
	int TeamId;
};
