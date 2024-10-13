// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Woman.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/Sword.h"
#include "Animation/AnimMontage.h"


AWoman::AWoman()
{
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 550.0f, 0.0);
}

void AWoman::I_Set_AttackDirection(const FVector& AttackDirection)
{
}


void AWoman::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWoman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWoman::GetHit(const FVector_NetQuantize& ImpactPoint)
{
	DrawDebugSphere(GetWorld(), ImpactPoint, 15.0f, 12, FColor::Red);

	if (HittedMontage == nullptr) return;

	PlayAnimMontage(HittedMontage);
}

void AWoman::I_GetAttackDirection(const FVector& AttackDirection)
{
	auto Cross = FVector::CrossProduct(AttackDirection, GetActorForwardVector());
}

void AWoman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AddMappingContextForCharacter();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AWoman::Looking);
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AWoman::Moving);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AWoman::Jump);
	EnhancedInputComponent->BindAction(IA_Equip, ETriggerEvent::Started, this, &AWoman::ETrigger);
	EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AWoman::Attack);
}



void AWoman::AddMappingContextForCharacter()
{

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(InputMappingContext,0);
}

void AWoman::Looking(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if (LookValue.X != 0.0)
		AddControllerYawInput(LookValue.X);

	if (LookValue.Y != 0.0)
		AddControllerPitchInput(LookValue.Y);

	bUseControllerRotationYaw = false;
}

void AWoman::Moving(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAC_Attack || bIsAction) return;

	const FVector2D MoveValue = Value.Get<FVector2D>();
	
	FRotator ControllerRotation = FRotator(0.0, GetControlRotation().Yaw,0.0);

	if(MoveValue.Y!=0)
		AddMovementInput(ControllerRotation.RotateVector(FVector::ForwardVector), MoveValue.Y);

	if (MoveValue.X != 0)
		AddMovementInput(ControllerRotation.RotateVector(FVector::RightVector), MoveValue.X);
}

void AWoman::Jump()
{
	if (ActionState == EActionState::EAC_Attack) return;

	Super::Jump();
}

void AWoman::ETrigger()
{
	if (bIsHaveWeapon == false)
	{
		Sword = Cast<ASword>(OverlapItem);

		if (Sword == nullptr) return;
		
		Sword->Equip(GetMesh(), FName("Right_HandSocket"),this,this);

		CharacterState = ECharacterState::ESC_EquippedOneHanedWeapon;

		bIsHaveWeapon = true;
		
	}


	else if (CharacterState == ECharacterState::ESC_EquippedOneHanedWeapon && bIsHaveWeapon && ActionState == EActionState::EAS_Unoccupied)
	{
		DisArm();
	}
	else if (CharacterState == ECharacterState::ECS_Unequipped && bIsHaveWeapon && ActionState == EActionState::EAS_Unoccupied)
	{
		Arm();
	}
}

void AWoman::Arm()
{
	if (AM_Arm)
	{
		PlayAnimMontage(AM_Arm);
		CharacterState = ECharacterState::ESC_EquippedOneHanedWeapon;
		bIsAction = true;
		//Sword->Equip(GetMesh(), FName("Right_HandSocket"));
	}

}

void AWoman::DisArm()
{
	if (AM_DisArm)
	{
		PlayAnimMontage(AM_DisArm);
		CharacterState = ECharacterState::ECS_Unequipped;
		bIsAction = true;
		//Sword->Equip(GetMesh(), FName("BackSocket"));
	}

}

void AWoman::Attack()
{
	if (IsAttack)
	{
		bCanCombo = true;
	}

	if (CharacterState == ECharacterState::ESC_EquippedOneHanedWeapon && ActionState != EActionState::EAC_Attack && IsAttack == false)
	{
		ActionState = EActionState::EAC_Attack;

		if (Attack_AM.Num() == 0) return;

		PlayAnimMontage(Attack_AM[AttackIndex]);
		IsAttack = true;
	}
}
