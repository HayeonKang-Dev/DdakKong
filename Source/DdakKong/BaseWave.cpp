// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWave.h"


// 배경으로 이동할 파도 (sin wave 적용)

// Sets default values
ABaseWave::ABaseWave()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseWave::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation(); 
	
}

void ABaseWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime; 

	FVector NewLocation = GetActorLocation(); 
	NewLocation.X += MoveSpeed * Direction * DeltaTime; 
	NewLocation.Z = StartLocation.Z + FMath::Sin(ElapsedTime * 2.f) * MoveZ; 
	SetActorLocation(NewLocation); 

	if (FMath::Abs(NewLocation.X - StartLocation.X) >= MoveRangeX) Direction *= -1.f; 

}

