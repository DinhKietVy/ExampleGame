// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_EndAction.h"
#include "Character/BaseCharacter.h"
#include "Character/Enemy.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

void UAN_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr) return;

	Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	Enemy = Cast<AEnemy>(MeshComp->GetOwner());

	if (Character == nullptr) return;

	Character->bIsAction = false;
	if (Enemy)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Enemy->Get_Patrol());
		MoveRequest.SetAcceptanceRadius(15.f);
		Enemy->Get_AIController()->MoveTo(MoveRequest);
	}
}
