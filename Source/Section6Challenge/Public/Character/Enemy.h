// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "CharacterState.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UHealthBarComponent;
class UAttributeComponent;
class UPawnSensingComponent;
class AAIController;

UCLASS()
class SECTION6CHALLENGE_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector_NetQuantize& ImpactPoint) override;

	virtual void I_GetAttackDirection(const FVector& AttackDirection) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UAnimMontage* Get_Correct_Montage(const FVector& AttackDirection);

private:
	bool IsInRange(AActor* Target, float Radius);

	void PatrolWatingFinish();

	void MoveToTarget(AActor* Target);

private:
	UPROPERTY()
	FTimerHandle PatrolTimer;

	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrol;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* FrontHittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* BackHittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* RightHittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* LeftHittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;

	bool BisDead = false;

	bool BisArrived = false;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly)
	float RemoveHealthWidget = 500.f;

	UPROPERTY(EditAnywhere)
	float PatrolRadius = 200.f;

	UPROPERTY()
	int PatrolIndex = 0;

	UPROPERTY(VisibleAnywhere)
	AActor* Causer;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	FVector AttackDirectTion;

	UPROPERTY()
	AAIController* EnenmyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* Patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> Patrols;

public:
	FORCEINLINE bool Get_BisDead() { return BisDead; }
	FORCEINLINE void I_Set_AttackDirection(const FVector& AttackDirection) { AttackDirectTion = AttackDirection; }
};
