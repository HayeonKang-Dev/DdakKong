// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/SphereComponent.h"
#include "BaseTarget.h"

#include "BaseShooter.generated.h"

UCLASS()
class DDAKKONG_API ABaseShooter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseShooter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 GetScore() { return Score; }

	void AddScore(int32 Amount, FVector HitLocation); 
private:
	UFUNCTION()
	void Shoot(); 

	UPROPERTY(VisibleAnywhere)
	USphereComponent* GunShot; 

	UPROPERTY(VisibleAnywhere)
	int Score=0;


};
