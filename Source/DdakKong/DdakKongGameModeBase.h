// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DdakKongGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DDAKKONG_API ADdakKongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public: 
	ADdakKongGameModeBase(); 

	void TargetDestroyed(); 

    // PlayerController가 호출하여 게임 타이머를 시작시킬 함수
	void StartGameTimer();

    UFUNCTION(BlueprintCallable)
    int32 GetTimeRemaining() {return TimeRemaining; }

protected:	
	virtual void BeginPlay() override; 

    virtual void Tick(float DeltaTime) override; 

    // 게임 시간 (초)
    UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
    int32 GameTimeInSeconds = 30.0f;
    
    // 실시간으로 줄어드는 남은 시간 (UI에 표시될 변수)
	UPROPERTY(BlueprintReadOnly, Category = "Game Rules")
	int32 TimeRemaining;

private: 
	void SpawnNewTarget(); 

	void EndGame(); 

	// 스폰할 과녁 블루프린트. 에디터에서 설정할 수 있도록 UPROPERTY 지정
    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    TSubclassOf<AActor> TargetBlueprint;

    // 유지할 과녁의 최대 개수
    int32 MaxTargets = 5;

    // 현재 월드에 있는 과녁 개수
    int32 CurrentTargetCnt = 5;

    // 과녁을 스폰할 X좌표 범위
    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    float SpawnRangeX = 100.0f;

    

    // 게임 타이머 핸들
    FTimerHandle GameTimerHandle;
	
};
