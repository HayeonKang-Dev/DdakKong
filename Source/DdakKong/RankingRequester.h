// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "http.h"
#include "DKPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "RankingRequester.generated.h"



UCLASS()
class DDAKKONG_API ARankingRequester : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARankingRequester();

	UFUNCTION(BlueprintCallable, Category="Ranking")
	void RequestTop10Rankings(); 


private:
    // OnRankingsReceived 함수에서 파싱 성공 시 델리게이트를 호출
    void OnRankingsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

