// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/EnemyAnimInstance.h"
#include "Character/Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<AEnemy>(TryGetPawnOwner());

}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Enemy == nullptr) return;

	if (Enemy->Get_BisDead())
	{
		BisDead = true;
	}
}
