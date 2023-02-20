// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}

	if (MovementState == EMovementState::EMS_Attacking) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection("F", HitReactMontage);
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CharacterInfo.CharacterStat.Health = FMath::Clamp(CharacterInfo.CharacterStat.Health - DamageAmount, 0.f, CharacterInfo.CharacterStat.MaxHealth);

	if (CharacterInfo.CharacterStat.Health == 0.f)
	{
		// SetEnemyState(EEnemyState::EES_Dead);
		// Dead();
	}
	return DamageAmount;
}

FVector ABaseCharacter::GetDesiredVelocity(FVector Target)
{
	FVector Velocity = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Target);
	return Velocity;
}