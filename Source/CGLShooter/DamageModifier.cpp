// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageModifier.h"

void UDamageModifier::OnBuffBegin(UStatSystem* StatsComponent)
{
	Activated = true;
	Timer = Time;
	StatsComponent->SetDamage(Amount);
}

void UDamageModifier::OnBuffTick(float DeltaTime)
{
	if(Timer > 0)
	{
		Timer -= DeltaTime;
	}else
	{
		Timer = 0;
	}
}

void UDamageModifier::OnBuffEnd(UStatSystem* StatsComponent)
{
	if(Time > 0){
		StatsComponent->SetDamage(-Amount);
	}
}

float UDamageModifier::GetTimer()
{
	return Timer;
}
