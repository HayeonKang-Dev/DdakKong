// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "kismet/GameplayStatics.h"
#include "http.h"
#include "Blueprint/UserWidget.h"
#include "BaseShooter.h"
#include "DKPlayerController.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FRankData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Ranking")
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly, Category="Ranking")
	int32 Score; 
};

UCLASS()
class DDAKKONG_API ADKPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: // UI 블루프린트에서 호출할 함수들
	
	// Start 위젯에서 호출: 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void StartGame(FString InPlayerName);

	// Ending 위젯에서 호출: 랭킹 요청
	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void RequestAndShowRankings();

	// Ending 위젯에서 호출: 게임 재시작
	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void RestartGame();

public: // 게임 로직 관련 함수들

	// 게임모드에서 호출: 게임 종료 처리
	void HandleGameEnd();

	// BaseShooter에서 호출: 점수 갱신 및 UI 팝업 요청
	void OnScoreUpdated(FVector TargetLocation);

protected: // 에디터에서 설정할 위젯 블루프린트 변수들

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> StartWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> EndingWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> RankingWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ScorePopupWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> RankEntryWidgetClass;


private: // 내부 처리용 변수 및 함수

	// 현재 표시 중인 위젯 인스턴스
	UPROPERTY()
	UUserWidget* CurrentWidget;

	// 게임 데이터
	FString PlayerName;

	// 서버 통신 관련 함수
	void SendFinalScore();
	void OnRankingsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 랭킹 위젯을 생성하고 데이터를 채워서 표시하는 함수
	void DisplayRankingWidget(const TArray<FRankData>& RankList);
};
