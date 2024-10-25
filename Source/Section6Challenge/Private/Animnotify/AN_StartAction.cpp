// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_StartAction.h"
#include "Character/BaseCharacter.h"

void UAN_StartAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr) return;

	Character = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (Character == nullptr) return;

	Character->bIsAction = true;
}
