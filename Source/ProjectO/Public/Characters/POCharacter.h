// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterState.h"
#include "Data/CharacterInfo.h"
#include "POCharacter.generated.h"

UCLASS()
class PROJECTO_API APOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APOCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	class APOPlayerController* Controller;
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* Camera;

public:
	/** State */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMovementState MovementState = EMovementState::EMS_Running;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ECombatState CombatState = ECombatState::ECS_Unarmed;
	FDodgeDirection DodgeDirection;
	FName GetDodgeWay();

	UFUNCTION(BlueprintCallable)
	FVector GetDesiredVelocity();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AEnemy* LockedOnEnemy;

	/** Character Info */
	UPROPERTY(EditAnywhere)
	FCharacterInfo CharacterInfo;

	/** Callbacks by Input */
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void WalkRun();
	void Dodge();
	void Jump();
	void EquipUnequip();
	void Sprint();
	void SprintEnd();
	void LockOn();
	void ChangeLockOn();
	void Attack();

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Input_FB;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Input_RL;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AWeapon> WeaponToSpawn;
	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

	/** Montage */
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EquipMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* DodgeMontage;

	/** Notify Callback */
	UFUNCTION(BlueprintCallable)
	void AttachWeapon();

	/** Combo Attack */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	bool CanNextCombo = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 MaxCombo = 3;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentCombo = 0;

	UFUNCTION(BlueprintCallable)
	void AttackStartComboState();
	UFUNCTION(BlueprintCallable)
	void AttackEndComboState();

	int32 CalculateDamage();

	/** Sound */
	UPROPERTY(EditAnywhere)
	class USoundBase* HitSound;
	UPROPERTY(EditAnywhere)
	class USoundBase* DashSound;

	/** Particles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* DodgeEffect;

private:
	bool bCanSprint = true;
};
