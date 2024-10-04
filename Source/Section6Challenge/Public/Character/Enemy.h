// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UHealthBarComponent;
class UAttributeComponent;

UCLASS()
class SECTION6CHALLENGE_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector_NetQuantize& ImpactPoint) override;

	virtual void I_GetAttackDirection(const FVector& AttackDirection) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

private:
	UAnimMontage* HittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;

	bool BisDead = false;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

public:
	FORCEINLINE bool Get_BisDead() { return BisDead; }
};
