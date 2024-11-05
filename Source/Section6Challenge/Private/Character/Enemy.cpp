// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/Sword.h"
#include "Character/Woman.h"
#include "HUD/PlayerOverlay.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));

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
	if (Spawn_Sword)
	{
		Sword = GetWorld()->SpawnActor<ASword>(Spawn_Sword,GetActorLocation(),GetActorRotation());
		
		Sword->Equip(GetMesh(), FName("RightHandSocket"), this, this);

		Sword->SetOwner(this);
	}

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
		MoveToTarget(Patrol,15.f);
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

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RemoveHealthBar();

	if (Patrol && EnenmyController)
	{
		if (BisDead) return;

		MakeMovementStop();

		SeePlayer();

		OutOfAttackRange();

		Guarding();
	}
}

bool AEnemy::IsInRange(AActor* Target, float Radius)
{
	if (Target == nullptr) return false;

	const auto Distance = (Target->GetActorLocation() - GetActorLocation()).Size();

	return Distance <= Radius;
}

void AEnemy::PatrolWatingFinish()
{
	EnemyState = EEnemyState::EES_Patrol;
}

void AEnemy::AttackWaitingFinish()
{
	BisAttackWaitingFinish = true;
}

void AEnemy::MoveToTarget(AActor* Target, float Radius)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(Radius);
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
	if ((EnemyState == EEnemyState::EES_Chasing && Patrol->ActorHasTag(FName("Woman"))) || EnemyState == EEnemyState::EEC_Attacking)
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		if (Causer == nullptr) return;

		Causer = GetPlayerController();
		HealthBarWidget->SetVisibility(true);
		if (!IsInRange(Causer, AttackRadius))
		{
			MoveToTarget(Patrol, 75.f);
		}
		if (IsInRange(Causer, AttackRadius))
		{
			EnenmyController->StopMovement();
			Attack();
		}
		if (!IsInRange(Patrol, RemoveHealthWidgetRadius))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("lose interesting"));
			GetCharacterMovement()->MaxWalkSpeed = 175.f;
			if (Patrols.Num() > 0)
			{
				Patrol = Patrols[PatrolIndex];
				MoveToTarget(Patrol, 15.f);
			}
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
			MoveToTarget(Patrol,15.f);
			EnemyState = EEnemyState::EES_Waiting;
			BisArrived = false;
		}
	}
}

void AEnemy::OutOfAttackRange()
{
	if (EnemyState == EEnemyState::EEC_Attacking && Patrol->ActorHasTag(FName("Woman")) && !IsInRange(Patrol, AttackRadius+50.f))
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetWorldTimerManager().ClearTimer(AttackTimer);
		BisAttackWaitingFinish = true;
	}
}

void AEnemy::MakeMovementStop()
{
	if (bIsAction)
	{
		EnenmyController->StopMovement();
	}
}

AActor* AEnemy::GetPlayerController()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	return PlayerController->GetPawn();
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	if (EnemyState == EEnemyState::EEC_Attacking) return;
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	EnemyState = EEnemyState::EES_Chasing;
	Causer = GetPlayerController();
	Patrol = GetPlayerController();
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AEnemy::Die()
{
	Super::Die();
	BisDead = true;
	Sword->SetLifeSpan(5.f);

	if (Spawn_Enemy1 && Spawn_Enemy2)
	{
		FVector Location = GetActorLocation();
		Location.X += 50.f;
		AEnemy* EnemyTemp1 = GetWorld()->SpawnActor<AEnemy>(Spawn_Enemy1, Location, GetActorRotation());
		//EnemyTemp1->MoveToTarget(Causer, 75.f);
		Location.X += 50.f;
		AEnemy* EnemyTemp2 = GetWorld()->SpawnActor<AEnemy>(Spawn_Enemy2, Location, GetActorRotation());
		//EnemyTemp2->MoveToTarget(Causer, 75.f);
	}
}

FVector AEnemy::Get_Causer_Translation()
{
	if (Causer == nullptr) return FVector();

	if (Causer)
	{
		FVector Distance = (GetActorLocation() - Causer->GetActorLocation()).GetSafeNormal();
		Distance *= WarpTargetDistance;
		return Causer->GetActorLocation() + Distance;
	}
	return FVector();
}

FVector AEnemy::Get_Causer_Rotation()
{
	if (Causer == nullptr) return FVector();

	if (Causer)
	{
		return Causer->GetActorLocation();
	}
	return FVector();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (BisDead || DamageCauser->GetOwner()->ActorHasTag(FName("Enemy"))) return 0;

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
	
	if (AttributeComponent->Get_Health() == 0) 
	{
		Die();
		AWoman* Finisher = Cast<AWoman>(DamageCauser->GetOwner());
		if (Finisher)
		{
			Finisher->Update_KillCount();
			Finisher->Get_PlayerOverlay()->SetKillCount(Finisher->Get_KillCount());
		}
	}

	return DamageAmount;
}

void AEnemy::Attack()
{
	if (AttackMontages.Num() == 0 || bIsAction || !BisAttackWaitingFinish) return;
	BisAttackWaitingFinish = false;

	AttackIndex = FMath::RandRange(0, AttackMontages.Num()-1);
	PlayAnimMontage(AttackMontages[AttackIndex]);

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan,TEXT("Attack"));

	EnemyState = EEnemyState::EEC_Attacking;

	GetWorldTimerManager().SetTimer(AttackTimer,this,&AEnemy::AttackWaitingFinish,AttackingSpeed);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}