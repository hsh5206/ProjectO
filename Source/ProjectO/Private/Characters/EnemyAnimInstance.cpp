// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyAnimInstance.h"
#include "Characters/Enemy.h"
#include "Characters/EnemyBoss.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Cast<AEnemy>(TryGetPawnOwner()))
	{
		Character = Cast<AEnemy>(TryGetPawnOwner());
		if (Character)
		{
			Movement = Character->GetCharacterMovement();
		}
	}
	
	if (Cast<AEnemyBoss>(TryGetPawnOwner()))
	{
		Character = Cast<AEnemyBoss>(TryGetPawnOwner());
		if (Character)
		{
			Movement = Character->GetCharacterMovement();
		}
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