// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterState.h"
#include "POAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTO_API UPOAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMovementState MovementState = EMovementState::EMS_Running;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ECombatState CombatState = ECombatState::ECS_Unarmed;
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class APOCharacter* Character;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* Movement;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float Input_FB;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float Input_RL;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float Speed_FB;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float Speed_RL;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsMoving;
};
