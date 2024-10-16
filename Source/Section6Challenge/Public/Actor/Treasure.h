// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item.h"
#include "Treasure.generated.h"

class AWoman;

UCLASS()
class SECTION6CHALLENGE_API ATreasure : public AItem
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	virtual void BeginShpereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Gold")
	int Gold;

	FRotator Rotator = GetActorRotation();

	UPROPERTY(VisibleAnywhere)
	AWoman* Woman;

	UPROPERTY(VisibleAnywhere)
	bool bIsCanPickup = false;
};
