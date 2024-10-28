// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

struct Fvector;

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SECTION6CHALLENGE_API IHitInterface
{
	GENERATED_BODY()

public:
	virtual void I_Set_AttackDirection(const FVector& AttackDirection) = 0;

	virtual void GetHit(const FVector_NetQuantize& ImpactPoint, AActor* Hitter) = 0;
};
