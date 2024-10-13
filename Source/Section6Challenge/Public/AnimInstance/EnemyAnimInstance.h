// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class AEnemy;
class UCharacterMovementComponent;

UCLASS()
class SECTION6CHALLENGE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly)
	bool BisDead = false;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;
private:

	UPROPERTY()
	AEnemy* Enemy;

	UPROPERTY()
	UCharacterMovementComponent* EnemyMovement;


};
