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

	virtual void I_Set_AttackDirection(const FVector& AttackDirection) override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector_NetQuantize& ImpactPoint, AActor* Hitter) override;
protected:
	virtual void BeginPlay() override;

private:
	void Spawn_ActorTimer_Finish();
private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Actor")
	TSubclassOf<ATreasure> Treasure;

	FTimerHandle SpawnActorTimer;

	UPROPERTY(EditDefaultsOnly)
	float SpawnWaitTime = 2.f;

	bool bIsBreak = false;
};
