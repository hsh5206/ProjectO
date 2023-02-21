// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/CharacterInfo.h"
#include "CharacterState.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECTO_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	float CalculateDamage();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AWeapon> WeaponToSpawn;

public:	
	/** State */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMovementState MovementState = EMovementState::EMS_Running;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ECombatState CombatState = ECombatState::ECS_Unarmed;

	/** Character Info */
	UPROPERTY(EditAnywhere)
	FCharacterInfo CharacterInfo;
	UPROPERTY(EditAnywhere)
	class USoundBase* CharacterAttackSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* GetHitSound;
	UPROPERTY(EditAnywhere)
	class USoundBase* CharacterGetHitSound;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* GetHitParticle;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* GetHitMontage;

	/** Montage*/
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EquipMontage;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipUnequip();
	UPROPERTY(EditAnywhere)
	class UAnimMontage* DeathMontage;
	void Death();

	/** Notify Callback */
	UFUNCTION(BlueprintCallable)
	void AttachWeapon();

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AEnemy* LockedOnEnemy;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetDesiredVelocity(FVector Target);

	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
};
