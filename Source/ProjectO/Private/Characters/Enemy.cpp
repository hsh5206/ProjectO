// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "HUD/EnemyHealthBarWidgetComponent.h"
#include "HUD/LockedOnWidgetComponent.h"

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

	int32 SectionNum = FMath::RandRange(1, 3);
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

	if (AnimInstance && SkillMontage_1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill_1"));
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(SkillMontage_1);
	}
}

void AEnemy::AttackSkill_2_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && SkillMontage_2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill_2"));
		MovementState = EMovementState::EMS_Attacking;
		AnimInstance->Montage_Play(SkillMontage_2);
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

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);

	FVector goVector = UKismetMathLibrary::GetDirectionUnitVector(ImpactPoint, GetActorLocation());
	LaunchCharacter(FVector(goVector.X, goVector.Y, 0.f)*1000, false, false);
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
