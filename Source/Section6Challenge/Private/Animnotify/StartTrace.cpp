// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/StartTrace.h"
#include "Character/Woman.h"

void UStartTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp,  Animation,  EventReference);
	if (MeshComp == nullptr) return;

	Woman = Cast<AWoman>(MeshComp->GetOwner());

	if (Woman == nullptr) return;

	Woman->bCanTrace = true;
}
