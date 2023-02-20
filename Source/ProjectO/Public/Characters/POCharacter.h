// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "POCharacter.generated.h"

UCLASS()
class PROJECTO_API APOCharacter : public ABaseCharacter
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
	FDodgeDirection DodgeDirection;
	FName GetDodgeWay();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AEnemy* LockedOnEnemy;

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
	class UAnimMontage* DodgeMontage;

	/** Notify Callback */
	UFUNCTION(BlueprintCallable)
	void AttachWeapon();

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Input_FB;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Input_RL;

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

	/** Dodge */
	UPROPERTY(EditAnywhere)
	class USoundBase* DashSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* DodgeEffect;

	int32 CalculateDamage();

	virtual FVector GetDesiredVelocity(FVector None) override;

private:
	bool bCanSprint = true;
};
