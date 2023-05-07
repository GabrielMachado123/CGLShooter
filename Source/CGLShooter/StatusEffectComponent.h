// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectFactory.h"
#include "StatSystem.h"
#include "StatusEffect.h"
#include "Components/ActorComponent.h"
#include "StatusEffectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CGLSHOOTER_API UStatusEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusEffectsComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void TickArrays(float DeltaTime);
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void AddDamageIncrease(float TimeAmount, float BuffAmount);
	UFUNCTION()
	void SetStatsComponent(UStatSystem* StatsComponentToSet);

	UFUNCTION()
	void CleanBuffs();


protected:
	const FBuffFactory* BuffFactory;

	UPROPERTY()
	TArray<UObject*> CurrentBuffArray;
	TArray<IStatusEffect*> CurrentBuffInterfaceArray;
	UPROPERTY()
	bool bIsCleaning = false;

	UPROPERTY(VisibleAnywhere)
	float ArrayLength;
	UPROPERTY()
	UStatSystem* StatsComponent;
};
