// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTarget.h"

// 과녁 맞으면 파괴 

// Sets default values
ABaseTarget::ABaseTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ABaseTarget::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation(); 

	GameMode = Cast<ADdakKongGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); 

	// 랜덤한 움직임을 위해 
	Direction = FMath::RandBool() ? 1.f : -1.f; 
	MoveSpeed = FMath::RandRange(150.f, 300.f); 
	MoveDistance = FMath::RandRange(250.f, 500.f); 
}


void ABaseTarget::ApplyDamage(int32 Damage, FVector HitLocation, FVector HitNormal)
{
	HP -= Damage; 

	// 자국 적용

	// 과녁의 체력이 0 일 때 
	if (IsDead()) {
		// 게임모드에 알림 
		if (GameMode) 
		{
			GameMode->TargetDestroyed(); 
		}
		Destroy(); 
	}
}


// Called every frame
void ABaseTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation(); 

	// 이동할 거리 계산 
	float MoveOffset = MoveSpeed * DeltaTime * Direction; 

	CurrentLocation.X += MoveOffset; 
	SetActorLocation(CurrentLocation); 

	float DistanceFromStart = FVector::Dist(StartLocation, CurrentLocation); 

	if (DistanceFromStart > MoveDistance) {
		Direction *= -1; 
	}

}

