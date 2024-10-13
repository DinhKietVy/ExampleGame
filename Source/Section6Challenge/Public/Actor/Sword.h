// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item.h"
#include "Sword.generated.h"

class AWoman;
class UStartTrace;
class UBoxComponent;
class USceneComponent;
class AActor;

UCLASS()
class SECTION6CHALLENGE_API ASword : public AItem
{
	GENERATED_BODY()
public:
	ASword();
	void Equip(USceneComponent* Parent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
	virtual void BeginPlay() override;


protected:

	UFUNCTION()
	void OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* SwordBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* StartTrace;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndTrace;

	UPROPERTY()
	AWoman* Woman;

	UPROPERTY(EditDefaultsOnly)
	UStartTrace* Trace;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ActorIgnore;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ActorHitted;

	TArray<AActor*>* ActorHittedPointer;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

public:
	FORCEINLINE TArray<AActor*>* Get_ActorHitted() { return ActorHittedPointer; }
};
