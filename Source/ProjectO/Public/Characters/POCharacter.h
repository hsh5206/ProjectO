// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/AIHelperInterface.h"
#include "POCharacter.generated.h"

UCLASS()
class PROJECTO_API APOCharacter : public ABaseCharacter, public IAIHelperInterface
{
	GENERATED_BODY()

public:
	APOCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY()
	class UUserWidget* DeathWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> DeathWidgetClass;

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

	/** Callbacks by Input */
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void WalkRun();
	void Dodge();
	void Jump();
	void Sprint();
	void SprintEnd();
	void LockOn();
	void ChangeLockOn();
	void Attack();
	void Block();
	void BlockEnd();
	void UsePortion();
	void Interact();
	bool bInteract = false;

	void BlockBreaked();
	bool bIsBlockBreaked = false;
	FTimerHandle BlockResetTimer;
	void BlockReset();

	/** Montage */
	UPROPERTY(EditAnywhere)
	class UAnimMontage* DodgeMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* BlockingMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* BlockingBreakMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* PortionMontage;

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

	virtual FVector GetDesiredVelocity(FVector None) override;

	/** AIHelper */
	virtual bool IsAlive_Implementation() override;
	virtual bool IsPlayer_Implementation() override;

	/** Portion */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PortionNum = 3;
	UFUNCTION(BlueprintCallable)
	void UsePortionEnd();

	void TransEnded();
	virtual void Death() override;

private:
	bool bCanSprint = true;
};
