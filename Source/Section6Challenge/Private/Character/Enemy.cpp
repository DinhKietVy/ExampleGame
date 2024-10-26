// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "HUD/HealthBarComponent.h"
#include "Component/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	EnemyState = EEnemyState::EES_Patrol;
}

void AEnemy::AttackWaitingFinish()
{
	Attack();
}

void AEnemy::MoveToTarget(AActor* Target)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnenmyController->MoveTo(MoveRequest);
}

void AEnemy::RemoveHealthBar()
{
	if (Causer)
	{
		if (!IsInRange(Causer, RemoveHealthWidgetRadius))
		{
			Causer = nullptr;
			if (HealthBarWidget == nullptr) return;

			HealthBarWidget->SetVisibility(false);
		}
	}
}

void AEnemy::SeePlayer()
{
	if (EnemyState == EEnemyState::EES_Chasing && Patrol->ActorHasTag(FName("Woman")))
	{
		Causer = GetPlayerController();
		HealthBarWidget->SetVisibility(true);
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Patrol);
		MoveRequest.SetAcceptanceRadius(60.f);
		EnenmyController->MoveTo(MoveRequest);
		if (IsInRange(Patrol, AttackRadius))
		{
			GetWorldTimerManager().ClearTimer(PatrolTimer);
			Attack();
		}
		if (!IsInRange(Patrol, RemoveHealthWidgetRadius))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("lose interesting"));
			GetCharacterMovement()->MaxWalkSpeed = 175.f;
			Patrol = Patrols[PatrolIndex];
			MoveToTarget(Patrol);
			BisArrived = false;
			EnemyState = EEnemyState::EES_Patrol;
		}
	}
}

void AEnemy::Guarding()
{
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
}

void AEnemy::OutOfAttackRange()
{
	if (EnemyState == EEnemyState::EEC_Attacking && Patrol->ActorHasTag(FName("Woman")) && !IsInRange(Patrol, AttackRadius))
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
		
}

void AEnemy::MakeMovementStop()
{
	if (bIsAction)
	{
		EnenmyController->StopMovement();
	}
}

bool AEnemy::Isbehind()
{
	if (Causer == nullptr) return true;

	CauserDirection = UKismetMathLibrary::GetDirectionUnitVector(Causer->GetActorLocation(),GetActorLocation());
	CheckBehind = FVector::DotProduct(GetActorForwardVector(),CauserDirection);
	return CheckBehind>0.5;
}

AActor* AEnemy::GetPlayerController()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	return PlayerController->GetPawn();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RemoveHealthBar();

	if (Patrol && EnenmyController)
	{
		if (BisDead) return;

		SeePlayer();

		OutOfAttackRange();

		Guarding();

		MakeMovementStop();

		Isbehind();
	}
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	if (EnemyState == EEnemyState::EEC_Attacking) return;
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	EnemyState = EEnemyState::EES_Chasing;
	Patrol = GetPlayerController();
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AEnemy::Die()
{
	Super::Die();
	BisDead = true;
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (BisDead) return 0;

	Causer = EventInstigator->GetPawn();

	Patrol = Causer;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	if (Get_Correct_Montage(AttackDirectTion) == nullptr) return 0;

	if (AttributeComponent && HealthBarWidget)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHelthPercent());

		HealthBarWidget->SetVisibility(true);
	}

	PlayAnimMontage(Get_Correct_Montage(AttackDirectTion));

	if (EnemyState != EEnemyState::EEC_Attacking)
	{
		EnemyState = EEnemyState::EES_Chasing;
	}
	
	if (AttributeComponent->Get_Health() == 0) Die();

	return DamageAmount;
}

void AEnemy::Attack()
{
	if (AttackMontage == nullptr || Isbehind()) return;

	PlayAnimMontage(AttackMontage);

	GetWorldTimerManager().SetTimer(AttackTimer,this,&AEnemy::AttackWaitingFinish,AttackingSpeed);

	EnemyState = EEnemyState::EEC_Attacking;
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}