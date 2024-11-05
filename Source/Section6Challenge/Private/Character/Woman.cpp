// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Woman.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/AttributeComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Actor/Sword.h"


AWoman::AWoman()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 550.0f, 0.0);
	GetMesh()->SetGenerateOverlapEvents(true);

	bUseControllerRotationYaw = false;
}

void AWoman::BeginPlay()
{
	Super::BeginPlay();

	InitiallizePlayeroverlay();

	PlayerOverlay->SetHealthBar(AttributeComponent->GetHelthPercent());
	PlayerOverlay->SetStaminaBar(AttributeComponent->GetStaminaPercent());
	PlayerOverlay->SetKillCount(KillCount);
}

void AWoman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AttributeComponent && PlayerOverlay)
	{
		AttributeComponent->RegenStamina(DeltaTime);
		PlayerOverlay->SetStaminaBar(AttributeComponent->GetStaminaPercent());
	}
}

void AWoman::GetHit(const FVector_NetQuantize& ImpactPoint, AActor* Hitter)
{
	if (HittedMontage == nullptr || bIsDodging) return;

	PlayAnimMontage(HittedMontage);
	//CharacterState = ECharacterState::ESC_Beaten;
	ActionState = EActionState::EAS_Unoccupied;
}

float AWoman::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CharacterState == ECharacterState::ESC_Dodge || bIsDodging) return DamageAmount;

	if (AttributeComponent && PlayerOverlay)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);
		PlayerOverlay->SetHealthBar(AttributeComponent->GetHelthPercent());

		if (AttributeComponent->Get_Health()==0)
		{
			Die();
		}
	}
	return DamageAmount;
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
	EnhancedInputComponent->BindAction(IA_Dodge, ETriggerEvent::Started, this, &AWoman::Dodge);
	EnhancedInputComponent->BindAction(IA_QuitGame, ETriggerEvent::Started, this, &AWoman::QuitGame);
	EnhancedInputComponent->BindAction(IA_Replay, ETriggerEvent::Started, this, &AWoman::Replay);
}

void AWoman::Die()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	BisDead = true;
	FTimerHandle WaittoReplay;
	GetWorldTimerManager().SetTimer(WaittoReplay,this,&AWoman::PlayAgain,5.f);
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

	
}

void AWoman::Moving(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAC_Attack || bIsAction || CharacterState == ECharacterState::ESC_Beaten || bIsDodging || BisDead) return;

	const FVector2D MoveValue = Value.Get<FVector2D>();
	
	FRotator ControllerRotation = FRotator(0.0, GetControlRotation().Yaw,0.0);

	if(MoveValue.Y!=0)
		AddMovementInput(ControllerRotation.RotateVector(FVector::ForwardVector), MoveValue.Y);

	if (MoveValue.X != 0)
		AddMovementInput(ControllerRotation.RotateVector(FVector::RightVector), MoveValue.X);
}

void AWoman::Jump()
{
	if (ActionState == EActionState::EAC_Attack || bIsAction || CharacterState == ECharacterState::ESC_Beaten || bIsDodging) return;

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
		//Sword->Equip(GetMesh(), FName("Right_HandSocket"));
	}

}

void AWoman::DisArm()
{
	if (AM_DisArm)
	{
		PlayAnimMontage(AM_DisArm);
		CharacterState = ECharacterState::ECS_Unequipped;
		//Sword->Equip(GetMesh(), FName("BackSocket"));
	}

}

void AWoman::Attack()
{
	if (bIsDodging) return;
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

void AWoman::Dodge()
{
	if (DodgeMontage == nullptr || bIsDodging ||!bIsEnoughStamina(DodgeStaminaCost)) return;

	PlayAnimMontage(DodgeMontage);
	bIsDodging = true;
	AttributeComponent->ReceiveStaminaCost(DodgeStaminaCost);
	PlayerOverlay->SetStaminaBar(AttributeComponent->GetStaminaPercent());
}

void AWoman::QuitGame()
{
	/*APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->ConsoleCommand("quit");*/
	UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));
}

void AWoman::Replay()
{
	UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap"));
}

void AWoman::PlayAgain()
{
	UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap"));
}

void AWoman::InitiallizePlayeroverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());

		if (PlayerHUD)
		{
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
		}
	}
}

bool AWoman::bIsEnoughStamina(float StaminaCost)
{
	return AttributeComponent->Get_Stamina()>StaminaCost;
}
