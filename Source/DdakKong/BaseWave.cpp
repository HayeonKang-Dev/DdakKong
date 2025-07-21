// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWave.h"


// 배경으로 이동할 파도 (sin wave 적용)

// Sets default values
ABaseWave::ABaseWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseWave::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation(); 
	
}

// Called every frame
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

