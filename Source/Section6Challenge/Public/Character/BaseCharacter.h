// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

class UAnimMontage;

UCLASS()
class SECTION6CHALLENGE_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector_NetQuantize& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool bCanTrace = false;

	bool bCanCombo = false;

	bool IsAttack = false;

	bool bIsAction = false;

protected:
	virtual void BeginPlay() override;

	virtual void Attack();

	virtual void Die();

	UAnimMontage* Get_Correct_Montage(const FVector& AttackDirection);

protected:
	UPROPERTY()
	FVector AttackDirectTion;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* FrontHittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* BackHittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* RightHittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* LeftHittedMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

public:
	FORCEINLINE void I_Set_AttackDirection(const FVector& AttackDirection) { AttackDirectTion = AttackDirection; }
};
