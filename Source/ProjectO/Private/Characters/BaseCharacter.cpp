// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

#include "Items/Weapon.h"
#include "Components/BoxComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (WeaponToSpawn)
	{
		Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponToSpawn));
		Weapon->Equip(GetMesh(), FName("BackWeaponSocket"), this);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (MovementState == EMovementState::EMS_Death) return;

	if (GetHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GetHitSound, ImpactPoint);
	}
	if (GetHitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetHitParticle, ImpactPoint);
	}
	if (CharacterGetHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CharacterGetHitSound, ImpactPoint);
	}

	MovementState = EMovementState::EMS_GettingHit;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CharacterInfo.CharacterStat.Health = FMath::Clamp(CharacterInfo.CharacterStat.Health - DamageAmount, 0.f, CharacterInfo.CharacterStat.MaxHealth);

	if (CharacterInfo.CharacterStat.Health == 0.f)
	{
		Death();
	}
	return DamageAmount;
}

FVector ABaseCharacter::GetDesiredVelocity(FVector Target)
{
	FVector Velocity = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Target);
	return Velocity;
}

float ABaseCharacter::CalculateDamage()
{
	return (Weapon->Damage + CharacterInfo.CharacterStat.Power) * ((100 + CharacterInfo.CharacterStat.Agility) / 100);
}

void ABaseCharacter::EquipUnequip_Implementation()
{
	if (MovementState == EMovementState::EMS_Blocking) return;
	if (MovementState == EMovementState::EMS_Attacking) return;

	if (!Weapon) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		if (CombatState == ECombatState::ECS_Unarmed)
		{
			AnimInstance->Montage_Play(EquipMontage, 1.5f);
			AnimInstance->Montage_JumpToSection(FName("Equip"), EquipMontage);
			CombatState = ECombatState::ECS_Armed;
		}
		else
		{
			AnimInstance->Montage_Play(EquipMontage, 1.5f);
			AnimInstance->Montage_JumpToSection(FName("Unequip"), EquipMontage);
			CombatState = ECombatState::ECS_Unarmed;

			if (LockedOnEnemy)
			{
				LockedOnEnemy = nullptr;
			}
		}
	}
}

void ABaseCharacter::Death()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}

	Weapon->Destroy();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	auto controllerRef = GetController();
	GetController()->UnPossess();
	controllerRef->Destroy();
	SetLifeSpan(5.f);

	MovementState = EMovementState::EMS_Death;
}

void ABaseCharacter::AttachWeapon()
{
	if (CombatState == ECombatState::ECS_Armed || CombatState == ECombatState::ECS_LockOn)
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("HandWeaponSocket"));
	}
	else
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("BackWeaponSocket"));
	}
}

void ABaseCharacter::EnableWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (Weapon)
	{
		Weapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		Weapon->IgnoreActors.Empty();
	}
}