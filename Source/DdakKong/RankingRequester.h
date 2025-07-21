// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "http.h"
#include "DKPlayerController.h"
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
	void OnRankingsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); 



};
