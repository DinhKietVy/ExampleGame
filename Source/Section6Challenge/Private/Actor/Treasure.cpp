// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Treasure.h"
#include "UObject/UObjectBaseUtility.h"
#include "Character/Woman.h"
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
	//Super::BeginShpereOverlap(OverlappedComponent,OtherActor, OtherComp,  OtherBodyIndex,  bFromSweep,  SweepResult);

	Woman = Cast<AWoman>(OtherActor);

	if (Woman == nullptr) return;
	bIsCanPickup = true;

	if (bIsCanPickup == false) return;
	Destroy();

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TEXT("pickup actor"));
}

void ATreasure::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Super::EndSphereOverlap(OverlappedComponent,  OtherActor,  OtherComp, OtherBodyIndex);
	bIsCanPickup = false;
}
