// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_End_Attack.h"
#include "Character/Woman.h"
#include "Character/BaseCharacter.h"
#include "Actor/Sword.h"

void UAN_End_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify( MeshComp,Animation,EventReference);
	if (MeshComp == nullptr) return;

	Woman = Cast<AWoman>(MeshComp->GetOwner());
	Character = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->IsAttack = false;
		Character->bCanCombo = false;
		Character->bCanTrace = false;
	}

	if (Woman)
	{
	Woman->ActionState = EActionState::EAS_Unoccupied;
	Woman->AttackIndex = 0;
	Woman->Get_Sword()->Get_ActorHitted()->Empty();
	}

}
