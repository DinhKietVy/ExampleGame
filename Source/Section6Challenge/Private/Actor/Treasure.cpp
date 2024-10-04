// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Treasure.h"
#include "UObject/UObjectBaseUtility.h"
#include "TimerManager.h"

void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotator.Yaw += 0.5f;
	AddActorWorldRotation(Rotator,false);
}

void ATreasure::BeginPlay()
{
	Super::BeginPlay();

	Rotator = GetActorRotation();
}

void ATreasure::BeginShpereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Destroy();

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TEXT("pickup actor"));
}
