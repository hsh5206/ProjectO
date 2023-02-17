// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterState.h"
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

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* Camera;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMovementState MovementState = EMovementState::EMS_Running;

	UFUNCTION(BlueprintCallable)
	FVector GetDesiredVelocity();

	/** Callbacks by Input */
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void WalkRun();
	void Dodge();
	void Jump();

	float Input_FB;
	float Input_RL;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsMoving;
};
