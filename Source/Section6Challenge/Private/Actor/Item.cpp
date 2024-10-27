// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item.h"
#include "Section6Challenge/Section6Challenge.h"
#include "Character/Woman.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = Item;
	Item->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Item->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::EndSphereOverlap);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::BeginShpereOverlap);
}

void AItem::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWoman* Woman = Cast<AWoman>(OtherActor);
	if (Woman)
	{
		Woman->Set_Mesh(nullptr);
	}
}

void AItem::BeginShpereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWoman* Woman = Cast<AWoman>(OtherActor);
	if (Woman)
	{
		Woman->Set_Mesh(this);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	runningtime += DeltaTime;

	float Deltaz = BienDo * FMath::Sin(runningtime * time);

	AddActorWorldOffset(FVector(0.0f,0.0f, Deltaz));

	FVector Foward = GetActorForwardVector();
	FVector Location = GetActorLocation();
}

