// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/WomanAnimInstace.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/MovementComponent.h"
#include "Character/CharacterState.h"
#include "Character/Woman.h"
#include "GameFramework/CharacterMovementComponent.h"

void UWomanAnimInstace::NativeInitializeAnimation()
{
	Super:: NativeInitializeAnimation();
	Character = Cast<AWoman>(TryGetPawnOwner());

	if (Character)
		MovementComponent = Character->GetCharacterMovement();
}

void UWomanAnimInstace::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComponent == nullptr) return;

	GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	if (Character->Get_BisDead())
		BisDead = true;

	IsFalling = MovementComponent->IsFalling();

	if (GroundSpeed > 0)
		IsRunning = true;
	else IsRunning = false;

	CharacterState = Character->Get_Enum_CharacterState();
}
