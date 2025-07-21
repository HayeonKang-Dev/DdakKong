// Copyright Epic Games, Inc. All Rights Reserved.


#include "DdakKongGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADdakKongGameModeBase::ADdakKongGameModeBase()
{

}

void ADdakKongGameModeBase::BeginPlay() 
{
    Super::BeginPlay(); 

    for(int32 i=0; i<MaxTargets; ++i) 
    {
        SpawnNewTarget();
    }

    GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ADdakKongGameModeBase::EndGame, GameTimeInSeconds, false);

}

void ADdakKongGameModeBase::TargetDestroyed() 
{
    CurrentTargetCnt--; 
    SpawnNewTarget(); 
}

void ADdakKongGameModeBase::SpawnNewTarget() 
{
    if (TargetBlueprint == nullptr) 
    {
        return; 
    }

    if (CurrentTargetCnt < MaxTargets)
    {
        float RandomX = FMath::RandRange(-SpawnRangeX, SpawnRangeX); 
        float RandomY = FMath::RandRange(0.f, 20.f); 


        FVector SpawnLocation = FVector(RandomX, RandomY, 0.f); 
        FRotator SpawnRotation = FRotator::ZeroRotator; 

        GetWorld()->SpawnActor<AActor>(TargetBlueprint, SpawnLocation, SpawnRotation);
        CurrentTargetCnt++; 

    }
}

void ADdakKongGameModeBase::EndGame() 
{

}