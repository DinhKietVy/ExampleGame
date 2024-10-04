// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreackAbleActor.generated.h"

class UGeometryCollectionComponent;
class ATreasure;

UCLASS()
class SECTION6CHALLENGE_API ABreackAbleActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreackAbleActor();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector_NetQuantize& ImpactPoint) override;

	virtual void I_GetAttackDirection(const FVector& AttackDirection) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Actor")
	TSubclassOf<ATreasure> Treasure;

	bool bIsBreak = false;
};
