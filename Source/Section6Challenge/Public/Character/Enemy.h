// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterState.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UHealthBarComponent;
class UAttributeComponent;
class UPawnSensingComponent;
class AAIController;
class UAnimMontage;
class ASword;

UCLASS()
class SECTION6CHALLENGE_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack() override;

protected:
	virtual void BeginPlay() override;

	void Die() override;

private:
	bool IsInRange(AActor* Target, float Radius);

	void PatrolWatingFinish();

	void AttackWaitingFinish();

	void MoveToTarget(AActor* Target, float Radius);

	void RemoveHealthBar();

	void SeePlayer();

	void Guarding();

	void OutOfAttackRange();

	void MakeMovementStop();

	bool Isbehind();

	AActor* GetPlayerController();

private:
	UPROPERTY()
	FTimerHandle PatrolTimer;

	UPROPERTY()
	FTimerHandle AttackTimer;

	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrol;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;

	bool BisDead = false;

	bool BisArrived = false;

	UPROPERTY(VisibleAnywhere)
	bool BisAttackWaitingFinish = true;

	UPROPERTY(EditDefaultsOnly)
	float RemoveHealthWidgetRadius = 500.f;

	UPROPERTY(EditAnywhere)
	float PatrolRadius = 200.f;

	UPROPERTY()
	int PatrolIndex = 0;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
	float AttackingSpeed = 2.f;

	UPROPERTY(VisibleAnywhere)
	float CheckBehind;

	UPROPERTY(VisibleAnywhere)
	AActor* Causer;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	AAIController* EnenmyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* Patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> Patrols;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY()
	FVector CauserDirection;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ASword> Spawn_Sword;

	ASword* Sword;

public:
	FORCEINLINE bool Get_BisDead() { return BisDead; }
	FORCEINLINE AAIController* Get_AIController() { return EnenmyController; }
	FORCEINLINE AActor* Get_Patrol() { return Patrol; }
	FORCEINLINE ASword* Get_Sword() { return Sword; }
};
