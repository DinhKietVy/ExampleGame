// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BreackAbleActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Actor/Treasure.h"

ABreackAbleActor::ABreackAbleActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(GetRootComponent());

	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABreackAbleActor::I_Set_AttackDirection(const FVector& AttackDirection)
{
}

void ABreackAbleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreackAbleActor::Spawn_ActorTimer_Finish()
{
	UWorld* World = GetWorld();
	if (World == nullptr && Treasure == nullptr) return;
	FVector Location = GetActorLocation();
	Location.Z += 75.0f;

	World->SpawnActor<ATreasure>(Treasure, Location, GetActorRotation());

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TEXT("spawn actor"));
}

void ABreackAbleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreackAbleActor::GetHit(const FVector_NetQuantize& ImpactPoint)
{
	if (bIsBreak == true) return;

	bIsBreak = true;
	SetLifeSpan(5.0f);

	if (Treasure == nullptr) return;

	GetWorldTimerManager().SetTimer(SpawnActorTimer,this,&ABreackAbleActor::Spawn_ActorTimer_Finish,SpawnWaitTime);
}

void ABreackAbleActor::I_GetAttackDirection(const FVector& AttackDirection)
{
}

