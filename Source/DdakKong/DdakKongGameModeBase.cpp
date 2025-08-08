// Copyright Epic Games, Inc. All Rights Reserved.


#include "DdakKongGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "DKPlayerController.h"
#include "TimerManager.h"

ADdakKongGameModeBase::ADdakKongGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
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

void ADdakKongGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	// 게임 타이머가 활성화되어 있다면 남은 시간을 계속 갱신
	if (GameTimerHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(GameTimerHandle))
	{
		TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(GameTimerHandle);
	}
}

void ADdakKongGameModeBase::TargetDestroyed() 
{
    CurrentTargetCnt--; 
    SpawnNewTarget(); 
}

void ADdakKongGameModeBase::StartGameTimer()
{
    // 설정된 시간 후에 EndGame 함수를 한 번 호출하는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ADdakKongGameModeBase::EndGame, GameTimeInSeconds, false);

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
    // PlayerController의 게임 종료 처리 함수 호출
	ADKPlayerController* PC = Cast<ADKPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->HandleGameEnd();
	}
}