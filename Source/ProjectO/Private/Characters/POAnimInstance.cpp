// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/POAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/POCharacter.h"

void UPOAnimInstance::NativeInitializeAnimation()
{
	Character = Cast<APOCharacter>(TryGetPawnOwner());
	if (Character)
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UPOAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Movement)
	{
		bIsInAir = Movement->IsFalling();
	}
	
	if (Character)
	{
		Input_FB = Character->Input_FB;
		Input_RL = Character->Input_RL;

		MovementState = Character->MovementState;

		if (Movement)
		{
			if (Input_FB == 0.f && Input_RL == 0.f)
			{
				bIsMoving = false;
			}
			else {
				bIsMoving = true;
			}
		}
	}

}