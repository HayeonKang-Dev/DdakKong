// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"


// 과녁 관리 
// 게임 내 항상 과녁의 수가 5개로 유지되게 함 (과녁 스폰)


AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

