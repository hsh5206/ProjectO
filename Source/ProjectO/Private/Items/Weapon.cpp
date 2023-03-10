// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/BaseCharacter.h"
#include "Interfaces/HitInterface.h"
#include "Characters/EnemyAIController.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(GetRootComponent());
	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Equip(USceneComponent* InParent, const FName& InSocketName, APawn* NewInstigator)
{
	SetInstigator(NewInstigator);
	SetOwner(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	Mesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/**
	* Trace
	*/
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(Cast<AActor>(GetOwner()));

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		TraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);

	int32 TotalDamage = Cast<ABaseCharacter>(GetOwner())->CalculateDamage();

	if (BoxHit.GetActor())
	{
		
		if (GetOwner()->ActorHasTag(FName("Player")))
		{
			if (BoxHit.GetActor()->ActorHasTag(FName("Enemy")))
			{
				UGameplayStatics::ApplyDamage(BoxHit.GetActor(),
					TotalDamage,
					GetInstigator()->GetController(),
					this,
					UDamageType::StaticClass()
				);

				IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
				if (HitInterface)
				{
					HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
				}
				IgnoreActors.AddUnique(BoxHit.GetActor());
			}
		}
		else if(GetOwner()->ActorHasTag(FName("Enemy")))
		{
			if (BoxHit.GetActor()->ActorHasTag(FName("Player")))
			{
				if (Cast<ABaseCharacter>(BoxHit.GetActor())->MovementState == EMovementState::EMS_GettingHit) return;

				UGameplayStatics::ApplyDamage(BoxHit.GetActor(),
					TotalDamage,
					GetInstigator()->GetController(),
					this,
					UDamageType::StaticClass()
				);

				IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
				if (HitInterface)
				{
					HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
				}
				IgnoreActors.AddUnique(BoxHit.GetActor());
			}
		}
	}
}
