// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "HUD/EnemyHealthBarWidgetComponent.h"
#include "HUD/LockedOnWidgetComponent.h"
#include "Items/Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HealthBarWidget = CreateDefaultSubobject<UEnemyHealthBarWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	LockedOnImage = CreateDefaultSubobject<ULockedOnWidgetComponent>(TEXT("LockedOnImage"));
	LockedOnImage->SetupAttachment(GetRootComponent());
	LockedOnImage->SetVisibility(false);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::AttackBasic_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GetHitMontage && AnimInstance->Montage_IsPlaying(GetHitMontage))
	{
		AnimInstance->Montage_Stop(0.f, GetHitMontage);
	}

	int32 SectionNum = FMath::RandRange(1, 2);
	FName SectionName = *FString::Printf(TEXT("Attack%d"), SectionNum);

	if (AnimInstance && AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("BasicAttack"));
		LaunchCharacter(GetActorForwardVector() * 1000, false, false);
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemy::AttackSkill_1_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GetHitMontage && AnimInstance->Montage_IsPlaying(GetHitMontage))
	{
		AnimInstance->Montage_Stop(0.f, GetHitMontage);
	}

	if (AnimInstance && SkillMontage_1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill_1"));
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(SkillMontage_1);
	}
}

void AEnemy::AttackEnd()
{
	MovementState = EMovementState::EMS_Running;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthBarWidget)
	{
		float Percent = float(CharacterInfo.CharacterStat.Health) / float(CharacterInfo.CharacterStat.MaxHealth);
		HealthBarWidget->SetHealthPercent(Percent);
	}

	return DamageAmount;
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);

	FVector goVector = UKismetMathLibrary::GetDirectionUnitVector(ImpactPoint, GetActorLocation());
	LaunchCharacter(FVector(goVector.X, goVector.Y, 0.f) * 1000, false, false);

	if (MovementState == EMovementState::EMS_Attacking) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage)) return;

	int32 SectionNum = FMath::RandRange(1, 2);
	FName SectionName = *FString::Printf(TEXT("Hit%d"), SectionNum);

	if (AnimInstance && GetHitMontage)
	{
		AnimInstance->Montage_Play(GetHitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, GetHitMontage);
	}
}