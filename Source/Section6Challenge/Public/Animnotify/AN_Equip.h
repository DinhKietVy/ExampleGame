// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Equip.generated.h"

class ASword;
class AWoman;

UCLASS()
class SECTION6CHALLENGE_API UAN_Equip : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	ASword* Sword;

	UPROPERTY()
	AWoman* Character;
};
