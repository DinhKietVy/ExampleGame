// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SECTION6CHALLENGE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	float BienDo;

	UPROPERTY(EditDefaultsOnly)
	float time;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	virtual void BeginShpereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Item;

	USphereComponent* Sphere;
private:	
	float runningtime;

	UPROPERTY(EditDefaultsOnly)
	FColor Color;
};
