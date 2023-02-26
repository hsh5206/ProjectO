// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SkillActor/BossSkillActor_Hammer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

#include "Characters/EnemyBoss.h"

ABossSkillActor_Hammer::ABossSkillActor_Hammer()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ItemMesh->SetupAttachment(GetRootComponent());
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprints/Enemy/Boss/SkillActor/Decal_Curve.Decal_Curve"));
	check(Curve.Succeeded());
	FloatCurve = Curve.Object;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

void ABossSkillActor_Hammer::BeginPlay()
{
	Super::BeginPlay();

	Spawn();
}

void ABossSkillActor_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timeline)
	{
		Timeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}

	/*if (!isAttacked)
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), GetOwner()->GetActorRotation(), DeltaTime, 0.f));
	}*/
}

void ABossSkillActor_Hammer::PlayTimeline()
{
	if (Timeline)
	{
		Timeline->PlayFromStart();
	}
}

void ABossSkillActor_Hammer::TimelineCallback(float val)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), val);
	Cast<UMaterialInstanceDynamic>(Decal->GetDecalMaterial())->SetScalarParameterValue(FName("OP"), val);
}

void ABossSkillActor_Hammer::TimelineFinishedCallback()
{

}

void ABossSkillActor_Hammer::Spawn()
{
	/*bRotate = true;*/

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.Linkage = 0;
	Info.ExecutionFunction = FName("Rotate");
	UKismetSystemLibrary::MoveComponentTo(ItemMesh, ItemMesh->GetRelativeLocation(), FRotator(0.f, 0.f, 0.f), true, true, 1.f, false, EMoveComponentAction::Move, Info);
}

void ABossSkillActor_Hammer::Rotate()
{
	if (GetOwner())
	{
		if (Cast<AEnemyBoss>(GetOwner())->LockedOnEnemy)
		{
			AttackLocation = Cast<AEnemyBoss>(GetOwner())->LockedOnEnemy->GetTargetLocation();
			Decal = GetWorld()->SpawnActor<ADecalActor>(FVector(AttackLocation.X, AttackLocation.Y, AttackLocation.Z - 100.f), FRotator());

			if (Decal)
			{
				UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(M_UnderPlayerDecal, this);
				Decal->SetDecalMaterial(DynMaterial);

				Decal->SetLifeSpan(1.3f);
				Decal->GetDecal()->DecalSize = FVector(150.f, 150.f, 150.f);

				FOnTimelineFloat onTimelineCallback;
				FOnTimelineEventStatic onTimelineFinishedCallback;

				if (FloatCurve)
				{
					Timeline->SetTimelineLength(1.3f);

					onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
					onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });

					Timeline->AddInterpFloat(FloatCurve, onTimelineCallback);
					Timeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

					Timeline->PlayFromStart();
				}
			}


			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			Info.Linkage = 0;
			Info.ExecutionFunction = FName("Attack");

			FRotator Rotation = (ItemMesh->GetComponentLocation() - AttackLocation).GetSafeNormal().Rotation();
			FRotator RelativeRotationFromRoot = UKismetMathLibrary::InverseTransformRotation(DefaultRoot->GetComponentTransform(), Rotation);

			UKismetSystemLibrary::MoveComponentTo(ItemMesh, ItemMesh->GetRelativeLocation(), FRotator(0.f, -RelativeRotationFromRoot.Yaw, 0.f), true, true, 1.f, false, EMoveComponentAction::Move, Info);
		}
		else
		{
			SetLifeSpan(1.f);
		}
	}

}

void ABossSkillActor_Hammer::Attack()
{
	isAttacked = true;

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.Linkage = 0;
	Info.ExecutionFunction = FName("DestroySkill");

	FVector RelativLocationFromRoot = UKismetMathLibrary::InverseTransformLocation(DefaultRoot->GetComponentTransform(), AttackLocation);
	FRotator Rotation = ItemMesh->GetRelativeRotation();
	UKismetSystemLibrary::MoveComponentTo(ItemMesh, RelativLocationFromRoot, FRotator(-130.f, Rotation.Yaw, Rotation.Roll), true, true, 0.2f, false, EMoveComponentAction::Move, Info);
	
}

void ABossSkillActor_Hammer::DestroySkill()
{
	/** Trace */
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult WeaponResult;

	const bool IsHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		AttackLocation,
		AttackLocation,
		150.f,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		WeaponResult,
		true,
		FLinearColor::Gray,
		FLinearColor::Blue,
		1.f
	);

	if (IsHit)
	{
		if (WeaponResult.GetActor()->ActorHasTag(FName("Player")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit"));
			if (Cast<ABaseCharacter>(WeaponResult.GetActor())->MovementState == EMovementState::EMS_GettingHit) return;

			UGameplayStatics::ApplyDamage(
				WeaponResult.GetActor(),
				Damage,
				Cast<AEnemyBoss>(GetOwner())->Controller,
				this,
				UDamageType::StaticClass()
			);

			IHitInterface* HitInterface = Cast<IHitInterface>(WeaponResult.GetActor());
			if (HitInterface)
			{
				HitInterface->Execute_GetHit(WeaponResult.GetActor(), WeaponResult.ImpactPoint);
			}
			IgnoreActors.AddUnique(WeaponResult.GetActor());
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UnderPlayerParticle, AttackLocation, FRotator::ZeroRotator);
	SetLifeSpan(1.f);
}
