// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterState.h"
#include "Woman.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEnhancedInputComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class ATreasure;
class ASword;
struct FInputActionValue;

UCLASS()
class SECTION6CHALLENGE_API AWoman : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWoman();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector_NetQuantize& ImpactPoint) override;

	UPROPERTY()
	EActionState ActionState = EActionState::EAS_Unoccupied;

	int AttackIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<UAnimMontage*> Attack_AM;

	UPROPERTY()
	ASword* Sword;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
protected:


private: 

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Arm();

	void DisArm();

	void AddMappingContextForCharacter();

	void Looking(const FInputActionValue &Value);

	void Moving(const FInputActionValue& Value);

	void Jump() override;

	void ETrigger();

	void Attack() override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	UEnhancedInputComponent* EnhancedInput;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Equip;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Attack;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlapItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AM_Arm;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AM_DisArm;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* HittedMontage;

	bool bIsHaveWeapon = false;

public:
	FORCEINLINE void Set_Mesh(AItem* Item) { OverlapItem = Item; }

	FORCEINLINE ECharacterState Get_Enum_CharacterState() const { return CharacterState; }

	FORCEINLINE ASword* Get_Sword() { return Sword; }

};
