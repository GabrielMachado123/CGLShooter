// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsableSkill.generated.h"

class AShooter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsableSkill : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CGLSHOOTER_API IUsableSkill
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitializeSkill(AShooter* Playable, UWorld* World,int Team) = 0;
	virtual void UseSkill() = 0;
	virtual void CastSkill(UAnimMontage* AnimationToPlay, float CooldownModifier, float Damage) = 0;
};
