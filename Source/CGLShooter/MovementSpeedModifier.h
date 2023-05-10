// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffect.h"
#include "UObject/NoExportTypes.h"
#include "MovementSpeedModifier.generated.h"

/**
 * 
 */
UCLASS()
class CGLSHOOTER_API UMovementSpeedModifier : public UObject, public IStatusEffect
{
	GENERATED_BODY()

	UFUNCTION()
	virtual void OnBuffBegin(UStatSystem* StatsComponent) override;
	UFUNCTION()
	virtual void OnBuffTick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnBuffEnd(UStatSystem* StatsComponent) override;
	UFUNCTION()
	virtual bool GetActivated() override { return Activated; }
	UFUNCTION()
	virtual float GetTimer() override;
	
	virtual void SetTime(const float Value) override { Time = Value; }
	virtual void SetAmount(const float Value) override { Amount = Value; }


protected:
	float Time = 0;
	float Amount = 0;
	bool Activated = false;
	float Timer = 0;
	float TemporarySpeed;
	
};
