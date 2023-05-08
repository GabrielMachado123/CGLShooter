// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpeedModifier.h"

void UAttackSpeedModifier::OnBuffBegin(UStatSystem* StatsComponent)
{
	Activated = true;
	Timer = Time;
	StatsComponent->SetAttackSpeed(Amount);
}

void UAttackSpeedModifier::OnBuffTick(float DeltaTime)
{
	if(Timer > 0)
	{
		Timer -= DeltaTime;
	}else
	{
		Timer = 0;
	}
}

void UAttackSpeedModifier::OnBuffEnd(UStatSystem* StatsComponent)
{
	if(Time > 0){
		StatsComponent->SetAttackSpeed(-Amount);
	}
}

float UAttackSpeedModifier::GetTimer()
{
	return Timer;
}
