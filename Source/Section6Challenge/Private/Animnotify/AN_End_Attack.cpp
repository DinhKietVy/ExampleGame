// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_End_Attack.h"
#include "Character/Woman.h"

void UAN_End_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify( MeshComp,Animation,EventReference);
	if (MeshComp == nullptr) return;

	Character = Cast<AWoman>(MeshComp->GetOwner());

	if (Character)
	{
	Character->ActionState = EActionState::ECS_Unoccupied;
	Character->AttackIndex = 0;
	Character->IsAttack = false;
	Character->bCanCombo = false;
	Character->bCanTrace = false;
	}

}
