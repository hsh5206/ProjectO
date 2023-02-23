// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterState.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTO_API UEnemyAnimInstance : public UAnimInstance
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
	class ABaseCharacter* Character;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* Movement;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float Speed = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsInAir;
};
