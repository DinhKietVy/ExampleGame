// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StartTrace.generated.h"

class AWoman;

UCLASS()
class SECTION6CHALLENGE_API UStartTrace : public UAnimNotify
{
	GENERATED_BODY()
	

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	AWoman* Woman;
};
