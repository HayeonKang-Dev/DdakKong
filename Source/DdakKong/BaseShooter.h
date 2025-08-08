// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/SphereComponent.h"
#include "BaseTarget.h"

#include "BaseShooter.generated.h"

class ADKPlayerController; 

UCLASS()
class DDAKKONG_API ABaseShooter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseShooter();

protected:
	virtual void BeginPlay() override;

	ADKPlayerController* PC;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	int32 GetScore();

	void AddScore(int32 Amount, FVector HitLocation); 
private:
	UFUNCTION()
	void Shoot(); 

	UPROPERTY(VisibleAnywhere)
	USphereComponent* GunShot; 

	UPROPERTY(VisibleAnywhere)
	int Score=0;

	

};
