// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/POCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"

#include "Items/Weapon.h"

APOCharacter::APOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 270.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10.f;

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

	if (WeaponToSpawn)
	{
		Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponToSpawn));
		Weapon->Equip(GetMesh(), FName("BackWeaponSocket"), this);
	}
	
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

	if (CombatState == ECombatState::ECS_LockOn)
	{
		float distance = GetDistanceTo(LockedOnEnemy);
		if (distance > 1500.f)
		{
			CombatState = ECombatState::ECS_Armed;
			LockedOnEnemy = nullptr;
		}
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
	PlayerInputComponent->BindAction(FName("EquipUnequip"), EInputEvent::IE_Pressed, this, &APOCharacter::EquipUnequip);
	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Pressed, this, &APOCharacter::Sprint);
	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Released, this, &APOCharacter::SprintEnd);
	PlayerInputComponent->BindAction(FName("LockOn"), EInputEvent::IE_Pressed, this, &APOCharacter::LockOn);
	PlayerInputComponent->BindAction(FName("ChangeLockOn"), EInputEvent::IE_Pressed, this, &APOCharacter::ChangeLockOn);
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &APOCharacter::Attack);

}

void APOCharacter::MoveForward(float value)
{
	if (MovementState == EMovementState::EMS_Attacking)
	{
		Input_FB = 0.f;
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(1.f, AttackMontage);
	Input_FB = value;

	/** Dodge Direction */
	if (value > 0.f)
	{
		DodgeDirection.bFront = true;
		DodgeDirection.bBack = false;
	}
	else if (value < 0.f)
	{
		DodgeDirection.bFront = false;
		DodgeDirection.bBack = true;
	}
	else
	{
		DodgeDirection.bFront = false;
		DodgeDirection.bBack = false;
	}
}

void APOCharacter::MoveRight(float value)
{
	if (MovementState == EMovementState::EMS_Attacking)
	{
		Input_RL= 0.f;
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(1.f, AttackMontage);
	Input_RL = value;

	/** Dodge Direction */
	if (value > 0.f)
	{
		DodgeDirection.bRight = true;
		DodgeDirection.bLeft = false;
	}
	else if (value < 0.f)
	{
		DodgeDirection.bRight = false;
		DodgeDirection.bLeft = true;
	}
	else
	{
		DodgeDirection.bRight = false;
		DodgeDirection.bLeft = false;
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
	if (MovementState == EMovementState::EMS_Jumping) return;
	if (MovementState == EMovementState::EMS_Dodging) return;
	if (MovementState == EMovementState::EMS_Attacking) return;

	MovementState = EMovementState::EMS_Dodging;
	if (CombatState == ECombatState::ECS_Unarmed) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage);
		FName Way = GetDodgeWay();

		if (Way.IsValid())
		{
			AnimInstance->Montage_JumpToSection(Way, DodgeMontage);
		}
	}
}

FName APOCharacter::GetDodgeWay()
{
	if (CombatState != ECombatState::ECS_LockOn)
	{
		return FName("F");
	}

	FName Way;
	if (DodgeDirection.bFront)
	{
		Way = FName("F");
		if (DodgeDirection.bRight)
		{
			Way = FName("FR");
		}
		else if (DodgeDirection.bLeft)
		{
			Way = FName("FL");
		}
	}
	else if (DodgeDirection.bBack)
	{
		Way = FName("B");
		if (DodgeDirection.bRight)
		{
			Way = FName("BR");
		}
		else if (DodgeDirection.bLeft)
		{
			Way = FName("BL");
		}
	}
	else
	{
		if (DodgeDirection.bRight)
		{
			Way = FName("R");
		}
		else if (DodgeDirection.bLeft)
		{
			Way = FName("L");
		}
		else
		{
			Way = FName("F");
		}
	}
	return Way;
}

void APOCharacter::Jump()
{
	if (MovementState == EMovementState::EMS_Attacking) return;
	if (MovementState == EMovementState::EMS_Dodging)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Stop(0.f, DodgeMontage);
	}

	ACharacter::Jump();
	MovementState = EMovementState::EMS_Jumping;
}

void APOCharacter::EquipUnequip()
{
	if (!Weapon) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		if (CombatState == ECombatState::ECS_Unarmed)
		{
			AnimInstance->Montage_Play(EquipMontage, 1.5f);
			AnimInstance->Montage_JumpToSection(FName("Equip"), EquipMontage);
			CombatState = ECombatState::ECS_Armed;
		}
		else
		{
			AnimInstance->Montage_Play(EquipMontage, 1.5f);
			AnimInstance->Montage_JumpToSection(FName("Unequip"), EquipMontage);
			CombatState = ECombatState::ECS_Unarmed;

			if (LockedOnEnemy)
			{
				LockedOnEnemy = nullptr;
			}
		}
	}
}

void APOCharacter::EnableWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (Weapon)
	{
		Weapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		Weapon->IgnoreActors.Empty();
	}
}

void APOCharacter::AttachWeapon()
{
	if (CombatState == ECombatState::ECS_Armed || CombatState == ECombatState::ECS_LockOn)
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("HandWeaponSocket"));
	}
	else
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("BackWeaponSocket"));
	}
}

void APOCharacter::Sprint()
{
	if (CombatState == ECombatState::ECS_LockOn) return;
	MovementState = EMovementState::EMS_Sprinting;
}

void APOCharacter::SprintEnd()
{
	if (CombatState == ECombatState::ECS_LockOn) return;
	MovementState = EMovementState::EMS_Running;
}

void APOCharacter::LockOn()
{
	if (CombatState == ECombatState::ECS_LockOn)
	{
		CombatState = ECombatState::ECS_Armed;
		LockedOnEnemy = nullptr;
		return;
	}

	FVector Start = GetActorLocation();
	FVector End = UKismetMathLibrary::Add_VectorVector(UKismetMathLibrary::Multiply_VectorFloat(Camera->GetForwardVector(), 1000.f), Start);

	TArray<AActor*> ActorsToIgnore;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);

	FHitResult SphereHit;

	UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		400.f,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		SphereHit,
		true
	);

	if (SphereHit.GetActor())
	{
		if (CombatState == ECombatState::ECS_Unarmed)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && EquipMontage)
			{
				AnimInstance->Montage_Play(EquipMontage, 1.5f);
				AnimInstance->Montage_JumpToSection(FName("Equip"), EquipMontage);
			}
		}
		LockedOnEnemy = Cast<APawn>(SphereHit.GetActor());
		CombatState = ECombatState::ECS_LockOn;
	}
}

void APOCharacter::ChangeLockOn()
{
	if (CombatState != ECombatState::ECS_LockOn) return;

	FVector Start = GetActorLocation();
	FVector End = GetActorLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(LockedOnEnemy);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);

	FHitResult SphereHit;

	UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		800.f,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		SphereHit,
		true
	);

	if (SphereHit.GetActor())
	{
		LockedOnEnemy = Cast<APawn>(SphereHit.GetActor());
	}
}

void APOCharacter::Attack()
{
	if (MovementState == EMovementState::EMS_Dodging) return;
	if (MovementState == EMovementState::EMS_Attacking && !CanNextCombo) return;
	if (MovementState == EMovementState::EMS_Jumping) return;

	if (FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1))
	{
		CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	}
	CanNextCombo = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	FName SectionName = *FString::Printf(TEXT("Attack%d"), CurrentCombo);
	
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		MovementState = EMovementState::EMS_Attacking;
	}
}

void APOCharacter::AttackStartComboState()
{
	CanNextCombo = true;
}

void APOCharacter::AttackEndComboState()
{
	MovementState = EMovementState::EMS_Running;
	CanNextCombo = false;
	CurrentCombo = 0;
}

int32 APOCharacter::CalculateDamage()
{
	return int32((Weapon->Damage + CharacterInfo.CharacterStat.Power) * ((100 + CharacterInfo.CharacterStat.Agility) / 100));
}

FVector APOCharacter::GetDesiredVelocity()
{
	FRotator ControlRotation = GetControlRotation();
	FVector Velocity = \
		UKismetMathLibrary::Add_VectorVector(\
		UKismetMathLibrary::Multiply_VectorFloat(FVector(UKismetMathLibrary::GetForwardVector(ControlRotation)), Input_FB), \
		UKismetMathLibrary::Multiply_VectorFloat(FVector(UKismetMathLibrary::GetRightVector(ControlRotation)), Input_RL) \
		);
	Velocity.Normalize();
	return Velocity;
}