// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Animation/AnimMontage.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
	if (DeathMontage == nullptr) return;

	PlayAnimMontage(DeathMontage);

	SetLifeSpan(5.f);

	SetActorEnableCollision(false);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::GetHit(const FVector_NetQuantize& ImpactPoint, AActor* Hitter)
{
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

UAnimMontage* ABaseCharacter::Get_Correct_Montage(const FVector& AttackDirection)
{
	FVector Cross = FVector::CrossProduct(AttackDirection, GetActorForwardVector());

	double Dot = FVector::DotProduct(AttackDirection, GetActorForwardVector());

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Dot = %f"), Dot));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Cross.z = %f"), Cross.Z));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Cross.y = %f"), Cross.Y));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Cross.x = %f"), Cross.X));

	if (Dot < 1 && Dot > 0.5)
	{
		return BackHittedMontage;
	}
	else if (Dot > -1 && Dot < -0.5)
	{
		return FrontHittedMontage;
	}
	else if (Cross.Z > 0)
	{
		return RightHittedMontage;
	}
	else
	{
		return LeftHittedMontage;
	}
}