// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownReductionModifier.h"

void UCooldownReductionModifier::OnBuffBegin(UStatSystem* StatsComponent)
{
	Activated = true;
	Timer = Time;
	StatsComponent->SetCooldownReduction(Amount);
}

void UCooldownReductionModifier::OnBuffTick(float DeltaTime)
{
	if(Timer > 0)
	{
		Timer -= DeltaTime;
	}else
	{
		Timer = 0;
	}
}

void UCooldownReductionModifier::OnBuffEnd(UStatSystem* StatsComponent)
{
	if(Time > 0){
		StatsComponent->SetCooldownReduction(-Amount);
	}
}

float UCooldownReductionModifier::GetTimer()
{
	return Timer;
}
