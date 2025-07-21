// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShooter.h"
#include "DKPlayerController.h"
// PlayerController에서 입력이 들어오면, 총 발사
// 타격 판정
// 점수 관리 


// Sets default values
ABaseShooter::ABaseShooter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	


}

// Called when the game starts or when spawned
void ABaseShooter::BeginPlay()
{
	Super::BeginPlay();
	TArray<UActorComponent*> SphereComponents = GetComponentsByClass(USphereComponent::StaticClass());
	for (UActorComponent* Comp : SphereComponents)
	{
		if (Comp && Comp->GetName() == TEXT("Bullet"))
		{
			GunShot = Cast<USphereComponent>(Comp);
			break;
		}
	}
}

// Called every frame
void ABaseShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseShooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ABaseShooter::Shoot);

}

void ABaseShooter::AddScore(int32 Amount, FVector HitLocation)
{
	Score += Amount; 

	ADKPlayerController* PC = Cast<ADKPlayerController>(GetController()); 
	if (PC) 
	{
		PC->OnScoreUpdated(HitLocation); 
	}
	
}

void ABaseShooter::Shoot()
{

	if (!GunShot) return;
	FVector Start = GunShot->GetComponentLocation(); 
	FVector Forward = GunShot->GetRightVector(); 
	Forward = -Forward; // RightVector를 LeftVector로 변환 
	FVector End = Start + Forward * 1000.0f; 

	FHitResult Hit; 
	FCollisionQueryParams Params; 
	Params.AddIgnoredActor(this); 

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params)) {
		if (AActor* HitActor = Hit.GetActor()) {
			if (HitActor->ActorHasTag("Target"))
			{
				Score += 10; 
				UE_LOG(LogTemp, Warning, TEXT("Target is Hitted!"));
				ABaseTarget* Target = Cast<ABaseTarget>(HitActor); 
				if (Target) {
					Target->ApplyDamage(10, Hit.ImpactPoint, Hit.ImpactNormal); 
				}

			}
		}
 	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 15.0f, 0, 1.5f);
}
