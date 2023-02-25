// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/ProgressBar.h"

#include "HUD/LockedOnWidgetComponent.h"
#include "Characters/EnemyAIController.h"
#include "HUD/BossHealthBar.h"
#include "Characters/POCharacter.h"
#include "Characters/SkillActor/BossSkillActor_Hammer.h"
#include "HUD/TextViewWidget.h"
#include "TimerManager.h"

AEnemyBoss::AEnemyBoss()
{
	LockedOnImage = CreateDefaultSubobject<ULockedOnWidgetComponent>(TEXT("LockedOnImage"));
	LockedOnImage->SetupAttachment(GetRootComponent());

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	Tags.Add(FName("Enemy"));

}

void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAttackChecking)
	{
		AttackCheck(60.f);
	}

	if (bSkill1Checking)
	{
		AttackCheck(100.f);
	}

	if (bSkill2Checking)
	{
		Skill2AttackCheck();
	}
}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	LockedOnImage->SetVisibility(false);
	CharacterInfo.CharacterStat.MaxHealth = 1.f;
	CharacterInfo.CharacterStat.Health = 1.f;

	if (BossWidgetClass)
	{
		BossHealthWidget = CreateWidget<UBossHealthBar>(GetWorld() , BossWidgetClass);
		float Percent = CharacterInfo.CharacterStat.Health / CharacterInfo.CharacterStat.MaxHealth;
		SetBossHealth(Percent);
	}

	if (TextViewWidgetClass)
	{
		TextViewWidget = CreateWidget<UTextViewWidget>(GetWorld(), TextViewWidgetClass);
	}
}

void AEnemyBoss::SetBossHealth(float percent)
{
	if (BossHealthWidget)
	{
		BossHealthWidget->HealthBar->SetPercent(percent);
	}
}

void AEnemyBoss::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APOCharacter>(OtherActor))
	{
		if (BossHealthWidget)
		{
			BossHealthWidget->AddToViewport(1);
		}
	}
}

void AEnemyBoss::AttackCheck(float AttackRadius)
{
	FVector Start = GetMesh()->GetSocketLocation(FName("Weapon_01"));
	FVector End = GetMesh()->GetSocketLocation(FName("Weapon_02"));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult WeaponResult;

	const bool IsHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		AttackRadius,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
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
			if (Cast<ABaseCharacter>(WeaponResult.GetActor())->MovementState == EMovementState::EMS_GettingHit) return;

			UGameplayStatics::ApplyDamage(WeaponResult.GetActor(),
				Damage,
				GetInstigator()->GetController(),
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
}

void AEnemyBoss::Skill2AttackCheck()
{
	FVector Location = GetMesh()->GetSocketLocation(FName("Skill2"));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult WeaponResult;

	const bool IsHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		Location,
		Location,
		125.f,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
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
			if (Cast<ABaseCharacter>(WeaponResult.GetActor())->MovementState == EMovementState::EMS_GettingHit) return;

			UGameplayStatics::ApplyDamage(WeaponResult.GetActor(),
				Damage,
				GetInstigator()->GetController(),
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
}

void AEnemyBoss::Skill3AttackCheck()
{
	FVector Location = GetMesh()->GetSocketLocation(FName("root"));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult WeaponResult;

	const bool IsHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		Location,
		Location,
		400.f,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
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
			if (Cast<ABaseCharacter>(WeaponResult.GetActor())->MovementState == EMovementState::EMS_GettingHit) return;

			UGameplayStatics::ApplyDamage(WeaponResult.GetActor(),
				Damage,
				GetInstigator()->GetController(),
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
}

void AEnemyBoss::AttackBasic_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GetHitMontage && AnimInstance->Montage_IsPlaying(GetHitMontage))
	{
		AnimInstance->Montage_Stop(0.f, GetHitMontage);
	}

	int32 SectionNum = FMath::RandRange(1, 2);
	FName SectionName = *FString::Printf(TEXT("Attack%d"), SectionNum);

	if (AnimInstance && AttackBasicMontage)
	{
		LaunchCharacter(GetActorForwardVector() * 1000, false, false);
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(AttackBasicMontage, 0.5f);
		AnimInstance->Montage_JumpToSection(SectionName, AttackBasicMontage);
	}
}

void AEnemyBoss::AttackSkill_1_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GetHitMontage && AnimInstance->Montage_IsPlaying(GetHitMontage))
	{
		AnimInstance->Montage_Stop(0.f, GetHitMontage);
	}

	if (AnimInstance && SkillMontage1)
	{
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(SkillMontage1);
	}
}

void AEnemyBoss::AttackSkill_2_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GetHitMontage && AnimInstance->Montage_IsPlaying(GetHitMontage))
	{
		AnimInstance->Montage_Stop(0.f, GetHitMontage);
	}

	if (AnimInstance && SkillMontage2)
	{
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(SkillMontage2, 0.8f);
	}
}

void AEnemyBoss::AttackSkill_3_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GetHitMontage && AnimInstance->Montage_IsPlaying(GetHitMontage))
	{
		AnimInstance->Montage_Stop(0.f, GetHitMontage);
	}

	if (AnimInstance && SkillMontage3)
	{
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(SkillMontage3);
	}
}

void AEnemyBoss::AttackSkill_4_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GetHitMontage && AnimInstance->Montage_IsPlaying(GetHitMontage))
	{
		AnimInstance->Montage_Stop(0.f, GetHitMontage);
	}

	if (AnimInstance && SkillMontage4)
	{
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(SkillMontage4);

		if (SkillActor4)
		{
			SpawnSkill4Actor();
		}
	}
}

void AEnemyBoss::SpawnSkill4Actor()
{
	Skill4ActorNum += 1;

	FVector Location = GetMesh()->GetSocketLocation(FName("root"));
	FRotator Rotation = GetMesh()->GetSocketRotation(FName("root"));

	if (Skill4ActorNum == 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnSkill4ActorTimer);
		AActor* Skill = GetWorld()->SpawnActor<AActor>(SkillActor4, GetMesh()->GetSocketLocation(FName("root")), Rotation);
		Skill->SetOwner(this);
		GetWorld()->GetTimerManager().SetTimer(SpawnSkill4ActorTimer, this, &AEnemyBoss::SpawnSkill4Actor, 0.5f, false);

		//Skill->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (Skill4ActorNum == 2)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnSkill4ActorTimer);

		AActor* Skill = GetWorld()->SpawnActor<AActor>(SkillActor4, FVector(Location.X, Location.Y - 70.f, Location.Z), Rotation);
		Skill->SetOwner(this);
		GetWorld()->GetTimerManager().SetTimer(SpawnSkill4ActorTimer, this, &AEnemyBoss::SpawnSkill4Actor, 0.5f, false);

		//Skill->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (Skill4ActorNum == 3)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnSkill4ActorTimer);

		AActor* Skill = GetWorld()->SpawnActor<AActor>(SkillActor4, FVector(Location.X, Location.Y + 70.f, Location.Z), Rotation);
		Skill->SetOwner(this);
		GetWorld()->GetTimerManager().SetTimer(SpawnSkill4ActorTimer, this, &AEnemyBoss::SpawnSkill4Actor, 0.5f, false);

		//Skill->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (Skill4ActorNum == 4)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnSkill4ActorTimer);

		AActor* Skill = GetWorld()->SpawnActor<AActor>(SkillActor4, FVector(Location.X, Location.Y - 140.f, Location.Z), Rotation);
		Skill->SetOwner(this);
		GetWorld()->GetTimerManager().SetTimer(SpawnSkill4ActorTimer, this, &AEnemyBoss::SpawnSkill4Actor, 0.5f, false);

		//Skill->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else if (Skill4ActorNum == 5)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnSkill4ActorTimer);

		AActor* Skill = GetWorld()->SpawnActor<AActor>(SkillActor4, FVector(Location.X, Location.Y + 140.f, Location.Z), Rotation);
		Skill->SetOwner(this);
		GetWorld()->GetTimerManager().SetTimer(SpawnSkill4ActorTimer, this, &AEnemyBoss::SpawnSkill4Actor, 0.5f, false);

		//Skill->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnSkill4ActorTimer);
		Skill4ActorNum = 0;
	}
}

void AEnemyBoss::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);

	/*FVector goVector = UKismetMathLibrary::GetDirectionUnitVector(ImpactPoint, GetActorLocation());
	LaunchCharacter(FVector(goVector.X, goVector.Y, 0.f) * 1000, false, false);*/

	if (MovementState == EMovementState::EMS_Attacking) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackBasicMontage && AnimInstance->Montage_IsPlaying(AttackBasicMontage)) return;
	if (AnimInstance && SkillMontage1 && AnimInstance->Montage_IsPlaying(SkillMontage1)) return;
	if (AnimInstance && SkillMontage2 && AnimInstance->Montage_IsPlaying(SkillMontage2)) return;
	if (AnimInstance && SkillMontage3 && AnimInstance->Montage_IsPlaying(SkillMontage3)) return;
	if (AnimInstance && SkillMontage4 && AnimInstance->Montage_IsPlaying(SkillMontage4)) return;
	if (AnimInstance && DeathMontage && AnimInstance->Montage_IsPlaying(DeathMontage)) return;

	FVector Forward = GetActorForwardVector();
	FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1;
	}

	FName Section("B");

	if (-45.f <= Theta && Theta < 45.f)
	{
		Section = FName("F");
	}
	else if (-135.f <= Theta && Theta < -45.f)
	{
		Section = FName("L");
	}
	else if (45.f <= Theta && Theta < 135.f)
	{
		Section = FName("R");
	}

	if (AnimInstance && GetHitMontage)
	{
		AnimInstance->Montage_Play(GetHitMontage);
		AnimInstance->Montage_JumpToSection(Section, GetHitMontage);
	}
}

float AEnemyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!LockedOnEnemy)
	{
		LockedOnEnemy = DamageCauser;
	}

	if (BossHealthWidget)
	{
		float Percent = CharacterInfo.CharacterStat.Health / CharacterInfo.CharacterStat.MaxHealth;
		SetBossHealth(Percent);
	}

	return DamageAmount;
}

void AEnemyBoss::Death()
{
	Super::Death();

	if (TextViewWidget)
	{
		TextViewWidget->AddToViewport();
		TextViewWidget->SetTextView(FText::FromString(FString::Printf(TEXT("Boss Slayered"))));
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyBoss::DeathFin, 3.f, false);
	}
}

void AEnemyBoss::DeathFin()
{
	UE_LOG(LogTemp, Warning, TEXT("Death"));
	GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	TextViewWidget->RemoveFromViewport();
}