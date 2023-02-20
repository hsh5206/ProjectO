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

protected:
	virtual void BeginPlay() override;

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
	class USoundBase* HitSound;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitReactMontage;

	/** Montage*/
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsMoving;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetDesiredVelocity(FVector Target);
};
