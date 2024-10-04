// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_Combo.h"
#include "Character/Woman.h"

void UAN_Combo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) return;

	Character = Cast<AWoman>(MeshComp->GetOwner());

	if (Character)
	{
		int Number = Character->Attack_AM.Num();
		Character->AttackIndex = (Character->AttackIndex + 1) % Number;
		if (Character->bCanCombo)
		{
			Character->bCanCombo = false;
			Character->PlayAnimMontage(Character->Attack_AM[Character->AttackIndex]);
		}

	}
}
