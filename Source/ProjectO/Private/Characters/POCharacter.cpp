// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/POCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"

APOCharacter::APOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 270.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 6.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 0.f);
}

void APOCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Input_FB == 0.f && Input_RL == 0.f)
	{
		bIsMoving = false;
	}
	else if (MovementState == EMovementState::EMS_Jumping || MovementState == EMovementState::EMS_Dodging)
	{
		bIsMoving = true;
	}
	else
	{
		bIsMoving = true;
	}
}

void APOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APOCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APOCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APOCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APOCharacter::Turn);

	PlayerInputComponent->BindAction(FName("ToggleWalkRun"), EInputEvent::IE_Pressed, this, &APOCharacter::WalkRun);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &APOCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Dodge"), EInputEvent::IE_Pressed, this, &APOCharacter::Dodge);

}

void APOCharacter::MoveForward(float value)
{
	Input_FB = value;
	if (MovementState == EMovementState::EMS_Jumping)
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, value, true);
	}
}

void APOCharacter::MoveRight(float value)
{
	Input_RL = value;
	if (MovementState == EMovementState::EMS_Jumping)
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, value);
	}
}

void APOCharacter::LookUp(float value)
{
	if (Controller && (value != 0.f))
	{
		AddControllerPitchInput(value);
	}
}

void APOCharacter::Turn(float value)
{
	if (Controller && (value != 0.f))
	{
		AddControllerYawInput(value);
	}
}

void APOCharacter::WalkRun()
{
	if (MovementState == EMovementState::EMS_Walking) MovementState = EMovementState::EMS_Running;
	else if (MovementState == EMovementState::EMS_Running) MovementState = EMovementState::EMS_Walking;
}

void APOCharacter::Dodge()
{
	MovementState = EMovementState::EMS_Dodging;
}

void APOCharacter::Jump()
{
	ACharacter::Jump();
	MovementState = EMovementState::EMS_Jumping;
}

FVector APOCharacter::GetDesiredVelocity()
{
	FRotator ControlRotation = GetControlRotation();
	FVector Velocity = \
		UKismetMathLibrary::Add_VectorVector(\
		UKismetMathLibrary::Multiply_VectorFloat(UKismetMathLibrary::GetForwardVector(ControlRotation), Input_FB), \
		UKismetMathLibrary::Multiply_VectorFloat(UKismetMathLibrary::GetRightVector(ControlRotation), Input_RL) \
		);

	return Velocity;
}