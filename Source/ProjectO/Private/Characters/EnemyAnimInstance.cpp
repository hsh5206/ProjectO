// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyAnimInstance.h"
#include "Characters/Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Character = Cast<AEnemy>(TryGetPawnOwner());
	if (Character)
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Movement)
	{
		bIsInAir = Movement->IsFalling();
		Speed = UKismetMathLibrary::VSizeXY(Movement->Velocity);
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
	}

	if (Character)
	{
		MovementState = Character->MovementState;
		CombatState = Character->CombatState;
	}

}