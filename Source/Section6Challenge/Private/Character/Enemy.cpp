// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "HUD/HealthBarComponent.h"
#include "Component/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));

	HealthBarWidget->SetupAttachment(RootComponent);

	PawnSensing->SetPeripheralVisionAngle(45.f);

	PawnSensing->SightRadius = 400.f;

	GetCharacterMovement()->bOrientRotationToMovement=true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 175.f;
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
	if (HealthBarWidget&&AttributeComponent)
	{
		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHelthPercent());
		HealthBarWidget->SetVisibility(false);
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//AActor* PlayerStartActor = PlayerController->GetPawn();
	//Patrol = PlayerStartActor;

	EnenmyController = Cast<AAIController>(GetController());
	
	if (EnenmyController && Patrol)
	{
		MoveToTarget(Patrol);
		//TArray<FNavPathPoint>& PathPoint = NavPatch->GetPathPoints();
		//for (auto& Point : PathPoint)
		//{
		//	const FVector& Location = Point.Location;
		//	DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Cyan,false ,3.f);
		//}
		EnemyState = EEnemyState::EES_Waiting;
		BisArrived = false;
	}
}

bool AEnemy::IsInRange(AActor* Target, float Radius)
{
	const auto Distance = (Target->GetActorLocation() - GetActorLocation()).Size();

	return Distance <= Radius;
}

void AEnemy::PatrolWatingFinish()
{
	if (EnemyState == EEnemyState::EES_Chasing) return;

	EnemyState = EEnemyState::EES_Patrol;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnenmyController->MoveTo(MoveRequest);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Causer)
	{
		if (!IsInRange(Causer, RemoveHealthWidget))
		{
			Causer = nullptr;
			if (HealthBarWidget == nullptr) return;
			HealthBarWidget->SetVisibility(false);
		}
	}

	if (Patrol && EnenmyController)
	{
		if (BisDead) return;

		if (EnemyState == EEnemyState::EES_Chasing) {
			MoveToTarget(Patrol);
			if (!IsInRange(Patrol, RemoveHealthWidget))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("lose interesting"));
				GetCharacterMovement()->MaxWalkSpeed = 175.f;
				Patrol = Patrols[PatrolIndex];
				MoveToTarget(Patrol);
				BisArrived = false;
				EnemyState = EEnemyState::EES_Patrol;
			}
		}

		if (IsInRange(Patrol, PatrolRadius) && !BisArrived)
		{
			BisArrived = true;
			GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolWatingFinish, 3.f);
		}
		if (IsInRange(Patrol, PatrolRadius) && EnemyState == EEnemyState::EES_Patrol)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("moving"));
			if (Patrols.Num() > 0)
			{
				PatrolIndex = (PatrolIndex + 1) % Patrols.Num();
				AActor* Target = Patrols[PatrolIndex];
				Patrol = Target;
				MoveToTarget(Patrol);
				EnemyState = EEnemyState::EES_Waiting;
				BisArrived = false;
			}
		}

		//if (!IsInRange(Patrol, PatrolRadius))
		//{
		//	FAIMoveRequest MoveRequest;
		//	MoveRequest.SetGoalActor(Patrol);
		//	MoveRequest.SetAcceptanceRadius(15.f);
		//	EnenmyController->MoveTo(MoveRequest);
		//}
	}
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	EnemyState = EEnemyState::EES_Chasing;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AActor* PlayerStart = PlayerController->GetPawn();
	Patrol = PlayerStart;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (BisDead) return 0;

	EnemyState = EEnemyState::EES_Chasing;

	Causer = EventInstigator->GetPawn();

	Patrol = Causer;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	if (AttributeComponent && HealthBarWidget)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHelthPercent());

		HealthBarWidget->SetVisibility(true);

		if (Get_Correct_Montage(AttackDirectTion) == nullptr) return 0;

		PlayAnimMontage(Get_Correct_Montage(AttackDirectTion));
	}

	if (AttributeComponent->Get_Health() == 0) Die();

	return DamageAmount;
}

void AEnemy::Die()
{
	Super::Die();
	BisDead = true;
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}