// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_EndAction.generated.h"

class AWoman;
class ABaseCharacter;
class AEnemy;

UCLASS()
class SECTION6CHALLENGE_API UAN_EndAction : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	ABaseCharacter* Character;

	UPROPERTY()
	AEnemy* Enemy;

	UPROPERTY()
	AWoman* Woman;
};
