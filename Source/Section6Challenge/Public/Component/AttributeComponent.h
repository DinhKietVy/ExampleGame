// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECTION6CHALLENGE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHelthPercent();

	void ReceiveDamage(float Damage);

	FORCEINLINE float Get_Health() { return Health; }
protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float Health = 100;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	int32 Gold;
};
