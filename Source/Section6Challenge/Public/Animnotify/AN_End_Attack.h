// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Character/Woman.h"
#include "AN_End_Attack.generated.h"

class AWoman;

UCLASS()
class SECTION6CHALLENGE_API UAN_End_Attack : public UAnimNotify
{
	GENERATED_BODY()
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	AWoman* Character;
};
