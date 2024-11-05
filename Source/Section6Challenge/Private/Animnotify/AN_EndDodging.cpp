// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_EndDodging.h"
#include "Character/Woman.h"
#include "Character/CharacterState.h"

void UAN_EndDodging::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Woman = Cast<AWoman>(MeshComp->GetOwner());
	if (Woman)
	{
		bool SetDodging = false;
		Woman->Set_bIsDodging(SetDodging);
	}
}
