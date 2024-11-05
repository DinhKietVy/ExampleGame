// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/Woman.h"
#include "WomanAnimInstace.generated.h"


class UCharacterMovementComponent;
class AWoman;

UCLASS()
class SECTION6CHALLENGE_API UWomanAnimInstace : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed= 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool IsRunning = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling = false;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly)
	bool BisDead = false;
private:

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY()
	AWoman* Character;


};
