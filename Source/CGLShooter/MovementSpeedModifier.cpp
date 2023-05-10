// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementSpeedModifier.h"

void UMovementSpeedModifier::OnBuffBegin(UStatSystem* StatsComponent)
{
	Activated = true;
	Timer = Time;
	StatsComponent->SetMovementSpeed(Amount);
}

void UMovementSpeedModifier::OnBuffTick(float DeltaTime)
{
	if(Timer > 0)
	{
		Timer -= DeltaTime;
	}else
	{
		Timer = 0;
	}
}

void UMovementSpeedModifier::OnBuffEnd(UStatSystem* StatsComponent)
{
	if(Time > 0){
		StatsComponent->SetMovementSpeed(-Amount);
	}
}

float UMovementSpeedModifier::GetTimer()
{
	return Timer;
}
