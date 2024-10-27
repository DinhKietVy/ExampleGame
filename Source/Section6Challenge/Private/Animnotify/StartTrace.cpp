// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/StartTrace.h"
#include "Character/BaseCharacter.h"
#include "Actor/Sword.h"
#include "Character/Woman.h"
#include "Character/Enemy.h"

void UStartTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp,  Animation,  EventReference);
	if (MeshComp == nullptr) return;

	Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	Woman = Cast<AWoman>(MeshComp->GetOwner());
	Enemy = Cast<AEnemy>(MeshComp->GetOwner());

	if (Character == nullptr) return;

	Character->bCanTrace = true;

	if (Woman)
	{
		Woman->Get_Sword()->Get_ActorHitted()->Empty();
	}

	if (Enemy)
	{
		Enemy->Get_Sword()->Get_ActorHitted()->Empty();
	}
}
