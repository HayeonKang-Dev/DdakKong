// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DdakKongGameModeBase.h"
#include "GameFramework/Actor.h"
#include "kismet/GameplayStatics.h"
#include "BaseTarget.generated.h"

UCLASS()
class DDAKKONG_API ABaseTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTarget();

	void ApplyDamage(int32 Damage, FVector HitLocation, FVector HitNormal);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsDead() { return HP <=0;}

	UPROPERTY(EditAnywhere, Category="Movement") 
	float MoveSpeed = 300.0f; 

	UPROPERTY(EditAnywhere, Category="Movement") 
	float MoveDistance = 500.0f; 

private: 
	FVector StartLocation; 
	
	// 이동 방향, 1=right
	int32 Direction = 1; 

	UPROPERTY(EditAnywhere)
	int HP = 20; 

	ADdakKongGameModeBase* GameMode; 

};
