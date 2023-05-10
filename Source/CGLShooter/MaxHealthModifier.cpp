// Fill out your copyright notice in the Description page of Project Settings.


#include "MaxHealthModifier.h"

void UMaxHealthModifier::OnBuffBegin(UStatSystem* StatsComponent)
{
	Activated = true;
	Timer = Time;
	StatsComponent->SetMaxHealth(Amount);
}

void UMaxHealthModifier::OnBuffTick(float DeltaTime)
{
	if(Timer > 0)
	{
		Timer -= DeltaTime;
	}else
	{
		Timer = 0;
	}
}

void UMaxHealthModifier::OnBuffEnd(UStatSystem* StatsComponent)
{
	if(Time > 0){
		StatsComponent->SetMaxHealth(-Amount);
	}
}

float UMaxHealthModifier::GetTimer()
{
	return Timer;
}
