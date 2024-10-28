// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Sword.h"
#include "Character/Woman.h"
#include "Character/BaseCharacter.h"
#include "Actor/Item.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Animnotify/StartTrace.h"
#include "Interface/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/BreackAbleActor.h"

ASword::ASword()
{
	SwordBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Sword Box"));
	SwordBox->SetupAttachment(GetRootComponent());
	
	SwordBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SwordBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SwordBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	StartTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Start Trace"));
	StartTrace->SetupAttachment(GetRootComponent());

	EndTrace = CreateDefaultSubobject<USceneComponent>(TEXT("End Trace"));
	EndTrace->SetupAttachment(GetRootComponent());
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	SwordBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBoxOverlapped);
	ActorHittedPointer = &ActorHitted;
}

void ASword::Equip(USceneComponent* Parent, FName SocketName,AActor* NewOwner, APawn* NewInstigator)
{
	if (NewOwner && NewInstigator)
	{
		SetOwner(NewOwner);
		SetInstigator(NewInstigator);
	}

	Character = Cast<ABaseCharacter>(Parent->GetOwner());
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	Item->AttachToComponent(Parent, TransformRules, SocketName);

	if (ActorIgnore.Contains(Character)) return;

	ActorIgnore.Add(Character);

	BienDo = 0;
	time = 0;

	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}



void ASword::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Character == nullptr || Character->bCanTrace == false) return;

	const FVector Start = StartTrace->GetComponentLocation();

	const FVector End = EndTrace->GetComponentLocation();

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(this,
		Start, End,
		FVector(2.5f,2.5f,2.5f),
		StartTrace->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,false,
		ActorIgnore,bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, BoxHit, true);

	if (BoxHit.GetActor())
	{
		if (ActorHittedPointer->Contains(BoxHit.GetActor()) || ActorHittedPointer == nullptr) return;

		ActorHittedPointer->Add(BoxHit.GetActor());
		const FVector AttackDirection = 
			UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), BoxHit.GetActor()->GetActorLocation());
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint,GetOwner());
			HitInterface->I_Set_AttackDirection(AttackDirection);
		}
		if (auto temp = Cast<ABreackAbleActor>(BoxHit.GetActor()))
		{
			CreateField(BoxHit.ImpactPoint);
		}
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage,
			GetInstigator()->GetController(), this,
			UDamageType::StaticClass());
	}
}
