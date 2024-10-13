// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_EndAction.h"
#include "Character/Woman.h"

void UAN_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) return;

	Character = Cast<AWoman>(MeshComp->GetOwner());

	if (Character == nullptr) return;

	Character->bIsAction = false;
}
