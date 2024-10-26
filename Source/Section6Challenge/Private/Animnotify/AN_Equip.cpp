// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_Equip.h"
#include "Character/Woman.h"
#include "Actor/Sword.h"

void UAN_Equip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);

	if (MeshComp == nullptr) return;

	Character = Cast<AWoman>(MeshComp->GetOwner());

	if (Character->Sword == nullptr) return;

	Sword = Character->Sword;

	if(Character->Get_Enum_CharacterState() == ECharacterState::ESC_EquippedOneHanedWeapon)
		Sword->Equip(MeshComp, FName("Right_HandSocket"),nullptr,nullptr);
	else if(Character->Get_Enum_CharacterState() == ECharacterState::ECS_Unequipped)
		Sword->Equip(MeshComp, FName("BackSocket"),nullptr,nullptr);
}
