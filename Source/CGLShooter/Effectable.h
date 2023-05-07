// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTestUtilityLibrary.h"
#include "StatusEffectComponent.h"
#include "UObject/Interface.h"
#include "Effectable.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEffectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CGLSHOOTER_API IEffectable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual UStatusEffectsComponent* IGetStatusEffectsComponent() = 0;
};
