// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWave.generated.h"

UCLASS()
class DDAKKONG_API ABaseWave : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWave();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartLocation; 

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 100.0f; 

	UPROPERTY(EditAnywhere)
	float MoveRangeX = 300.0f; 

	UPROPERTY(EditAnywhere)
	float MoveZ = 15.0f; 

	float Direction = 1.f; 
	float ElapsedTime = 0.f; 

};
