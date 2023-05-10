// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Modifier.generated.h"

UCLASS()
class CGLSHOOTER_API AModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	float Time;
	UPROPERTY(BlueprintReadWrite)
	float Amount;
	
	UFUNCTION(BlueprintCallable)
	void SetTime(float value) {Time = value;}
	UFUNCTION(BlueprintCallable)
	void SetAmount(float value) {Amount = value;}

};
