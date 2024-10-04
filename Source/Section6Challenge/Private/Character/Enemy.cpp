// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "HUD/HealthBarComponent.h"
#include "Component/AttributeComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));

	HealthBarWidget->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget&&AttributeComponent)
	{
		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHelthPercent());
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::GetHit(const FVector_NetQuantize& ImpactPoint)
{
	if (HittedMontage == nullptr) return;

	PlayAnimMontage(HittedMontage);
}

void AEnemy::I_GetAttackDirection(const FVector& AttackDirection)
{
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (BisDead) return 0;

	if (AttributeComponent && HealthBarWidget)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHelthPercent());
	}

	if (AttributeComponent->Get_Health() == 0)
	{
		if (DeathMontage == nullptr) return 0;
		
		PlayAnimMontage(DeathMontage);
		BisDead = true;

		SetLifeSpan(5.f);
	}

	return DamageAmount;
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

