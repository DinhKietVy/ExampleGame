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

protected:
	virtual void BeginPlay() override;

	void Die() override;

private:
	bool IsInRange(AActor* Target, float Radius);

	void PatrolWatingFinish();

	void MoveToTarget(AActor* Target);

private:
	UPROPERTY()
	FTimerHandle PatrolTimer;

	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrol;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;

	bool BisDead = false;

	bool BisArrived = false;


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
	AAIController* EnenmyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* Patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> Patrols;

public:
	FORCEINLINE bool Get_BisDead() { return BisDead; }
};
