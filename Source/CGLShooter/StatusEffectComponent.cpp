// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"

#include <array>

#include "AttackSpeedModifier.h"
#include "CooldownReductionModifier.h"
#include "DamageModifier.h"
#include "MaxHealthModifier.h"
#include "MovementSpeedModifier.h"
#include "StatusEffectFactory.h"


UStatusEffectsComponent::UStatusEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UStatusEffectsComponent::SetStatsComponent(UStatSystem* StatsComponentToSet)
{
	StatsComponent = StatsComponentToSet;
}

void UStatusEffectsComponent::CleanBuffs()
{
	if (CurrentBuffArray.Num() > 0 && CurrentBuffInterfaceArray.Num() > 0)
	{
		for (int i = CurrentBuffArray.Num() - 1; i >= 0; i--)
		{
			if (CurrentBuffArray.IsValidIndex(i) && CurrentBuffInterfaceArray.IsValidIndex(i))
			{
				CurrentBuffInterfaceArray[i]->OnBuffEnd(StatsComponent);
				CurrentBuffArray.RemoveAt(i);
				CurrentBuffInterfaceArray.RemoveAt(i);
			}
		}
	}
}

void UStatusEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	BuffFactory = new FBuffFactory();
	// CurrentBuffArray.Empty();
}

void UStatusEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickArrays(DeltaTime);
}

void UStatusEffectsComponent::TickArrays(float DeltaTime)
{
	if (CurrentBuffArray.Num() > 0 && CurrentBuffInterfaceArray.Num() > 0)
	{
		for (int i = CurrentBuffArray.Num() - 1; i >= 0; i--)
		{
			if (CurrentBuffArray.IsValidIndex(i) && CurrentBuffInterfaceArray.IsValidIndex(i))
			{
				CurrentBuffInterfaceArray[i]->OnBuffTick(DeltaTime);

				if (CurrentBuffInterfaceArray[i]->GetTimer() == 0)
				{
					CurrentBuffInterfaceArray[i]->OnBuffEnd(StatsComponent);
					CurrentBuffArray.RemoveAt(i);
					CurrentBuffInterfaceArray.RemoveAt(i);
				}
			}
			else
			{
				break;
			}
		}
	}

	ArrayLength = CurrentBuffArray.Num();
}

void UStatusEffectsComponent::AddDamageModifier(float TimeAmount, float BuffAmount)
{
	UObject* BuffToAdd = BuffFactory->CreateBuff<UDamageModifier>(TimeAmount, BuffAmount, StatsComponent);
	CurrentBuffArray.Add(BuffToAdd);
	CurrentBuffInterfaceArray.Add(Cast<IStatusEffect>(BuffToAdd));
}

void UStatusEffectsComponent::AddMaxHealthModifier(float TimeAmount, float BuffAmount)
{
	UObject* BuffToAdd = BuffFactory->CreateBuff<UMaxHealthModifier>(TimeAmount, BuffAmount, StatsComponent);
	CurrentBuffArray.Add(BuffToAdd);
	CurrentBuffInterfaceArray.Add(Cast<IStatusEffect>(BuffToAdd));
}

void UStatusEffectsComponent::AddAttackSpeedModifier(float TimeAmount, float BuffAmount)
{
	UObject* BuffToAdd = BuffFactory->CreateBuff<UAttackSpeedModifier>(TimeAmount, BuffAmount, StatsComponent);
	CurrentBuffArray.Add(BuffToAdd);
	CurrentBuffInterfaceArray.Add(Cast<IStatusEffect>(BuffToAdd));
}

void UStatusEffectsComponent::AddMovementSpeedModifier(float TimeAmount, float BuffAmount)
{
	UObject* BuffToAdd = BuffFactory->CreateBuff<UMovementSpeedModifier>(TimeAmount, BuffAmount, StatsComponent);
	CurrentBuffArray.Add(BuffToAdd);
	CurrentBuffInterfaceArray.Add(Cast<IStatusEffect>(BuffToAdd));
}

void UStatusEffectsComponent::AddCooldownReductionModifier(float TimeAmount, float BuffAmount)
{
	UObject* BuffToAdd = BuffFactory->CreateBuff<UCooldownReductionModifier>(TimeAmount, BuffAmount, StatsComponent);
	CurrentBuffArray.Add(BuffToAdd);
	CurrentBuffInterfaceArray.Add(Cast<IStatusEffect>(BuffToAdd));
}
