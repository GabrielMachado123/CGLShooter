// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageIncrease.h"

void UDamageIncrease::OnBuffBegin(UStatSystem* StatsComponent)
{
	Activated = true;
	Timer = Time;
	StatsComponent->SetDamage(Amount);
}

void UDamageIncrease::OnBuffTick(float DeltaTime)
{
	// if(Timer > 0)
	// {
	// 	Timer -= DeltaTime;
	// }else
	// {
	// 	Timer = 0;
	// }
}

void UDamageIncrease::OnBuffEnd(UStatSystem* StatsComponent)
{
	
}

float UDamageIncrease::GetTimer()
{
	return Timer;
}
