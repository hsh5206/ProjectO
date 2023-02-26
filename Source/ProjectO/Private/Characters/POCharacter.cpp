// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/POCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

#include "Items/Weapon.h"
#include "Characters/Enemy.h"
#include "Characters/EnemyBoss.h"
#include "HUD/LockedOnWidgetComponent.h"
#include "Characters/POPlayerController.h"
#include "HUD/EnemyHealthBarWidgetComponent.h"
#include "HUD/MainWidget.h"
#include "POGameInstance.h"

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

	DodgeEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DodgeEffect"));
	DodgeEffect->SetupAttachment(GetRootComponent());

	Tags.Add(FName("Player"));
}

void APOCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Controller = Cast<APOPlayerController>(GetController());
	DodgeEffect->Deactivate();

	UPOGameInstance* GI = Cast<UPOGameInstance>(GetGameInstance());
	GI->OnTransEndDelegate.AddDynamic(this, &APOCharacter::TransEnded);
	GI->LoadGameFromTrans(this, FString::Printf(TEXT("AutoSave")), 0);

	Controller->SetHealthPercent(CharacterInfo.CharacterStat.MaxHealth, CharacterInfo.CharacterStat.Health);
	Controller->SetStaminaPercent(CharacterInfo.CharacterStat.MaxStamina, CharacterInfo.CharacterStat.Stamina);
	Controller->SetPortionText(PortionNum);

	if (DeathWidgetClass)
	{
		DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
	}
}

void APOCharacter::TransEnded()
{
	
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

		if (LockedOnEnemy)
		{
			if (AEnemy* Enemy = Cast<AEnemy>(LockedOnEnemy))
			{
				if (Enemy->MovementState == EMovementState::EMS_Death)
				{
					Enemy->HealthBarWidget->SetVisibility(false);
					Enemy->LockedOnImage->SetVisibility(false);
					LockedOnEnemy = nullptr;
					ChangeLockOn();
					if (LockedOnEnemy == nullptr)
					{
						CombatState = ECombatState::ECS_Armed;
					}
				}
			}

			if (AEnemyBoss* Boss = Cast<AEnemyBoss>(LockedOnEnemy))
			{
				if (Boss->MovementState == EMovementState::EMS_Death)
				{
					Boss->LockedOnImage->SetVisibility(false);
					LockedOnEnemy = nullptr;
					ChangeLockOn();
					if (LockedOnEnemy == nullptr)
					{
						CombatState = ECombatState::ECS_Armed;
					}
				}
			}
		}
	}

	float DeltaStamina = 10.f * DeltaTime;


	if (bCanSprint && Controller && MovementState == EMovementState::EMS_Sprinting)
	{
		if (CharacterInfo.CharacterStat.Stamina < DeltaStamina)
		{
			MovementState = EMovementState::EMS_Running;
			bCanSprint = false;
			return;
		}

		CharacterInfo.CharacterStat.Stamina -= DeltaStamina;
		Controller->SetStaminaPercent(CharacterInfo.CharacterStat.MaxStamina, CharacterInfo.CharacterStat.Stamina);
	}

	else if (Controller && CharacterInfo.CharacterStat.Stamina != CharacterInfo.CharacterStat.MaxStamina)
	{
		if (MovementState == EMovementState::EMS_Blocking || bIsBlockBreaked) return;
		CharacterInfo.CharacterStat.Stamina += DeltaStamina;
		Controller->SetStaminaPercent(CharacterInfo.CharacterStat.MaxStamina, CharacterInfo.CharacterStat.Stamina);
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
	PlayerInputComponent->BindAction(FName("Block"), EInputEvent::IE_Pressed, this, &APOCharacter::Block);
	PlayerInputComponent->BindAction(FName("Block"), EInputEvent::IE_Released, this, &APOCharacter::BlockEnd);
	PlayerInputComponent->BindAction(FName("UsePortion"), EInputEvent::IE_Pressed, this, &APOCharacter::UsePortion);
	PlayerInputComponent->BindAction(FName("Interact"), EInputEvent::IE_Pressed, this, &APOCharacter::Interact);


}

void APOCharacter::Interact()
{
	bInteract = true;
}

void APOCharacter::MoveForward(float value)
{
	if (MovementState == EMovementState::EMS_Attacking)
	{
		Input_FB = 0.f;
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Stop(1.f, AttackMontage);
	}

	if (MovementState == EMovementState::EMS_Jumping)
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, value);
	}

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
		Input_RL = 0.f;
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Stop(1.f, AttackMontage);
	}

	Input_RL = value;

	if (MovementState == EMovementState::EMS_Jumping)
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, value);
	}

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
	if (CharacterInfo.CharacterStat.Stamina < 30) return;

	if (MovementState == EMovementState::EMS_Blocking) return;
	if (MovementState == EMovementState::EMS_Jumping) return;
	if (MovementState == EMovementState::EMS_Dodging) return;
	//if (MovementState == EMovementState::EMS_Attacking) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (EquipMontage && AnimInstance->Montage_IsPlaying(EquipMontage)) return;

	MovementState = EMovementState::EMS_Dodging;
	CharacterInfo.CharacterStat.Stamina -= 40.f;
	Controller->SetStaminaPercent(CharacterInfo.CharacterStat.MaxStamina, CharacterInfo.CharacterStat.Stamina);

	DodgeEffect->Activate();

	if (CombatState == ECombatState::ECS_Unarmed) return;

	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage);
		FName Way = GetDodgeWay();

		if (Way.IsValid())
		{
			AnimInstance->Montage_JumpToSection(Way, DodgeMontage);
		}
	}

	if (DashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
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
	if (MovementState == EMovementState::EMS_Blocking) return;
	if (MovementState == EMovementState::EMS_Attacking) return;

	if (MovementState == EMovementState::EMS_Dodging)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Stop(0.f, DodgeMontage);
	}

	ACharacter::Jump();
	MovementState = EMovementState::EMS_Jumping;
}

void APOCharacter::Sprint()
{
	if (MovementState == EMovementState::EMS_Blocking) return;
	if (CombatState == ECombatState::ECS_LockOn) return;

	if (CharacterInfo.CharacterStat.Stamina > 20.f) bCanSprint = true;

	if (bCanSprint)
	{
		MovementState = EMovementState::EMS_Sprinting;
	}
}

void APOCharacter::SprintEnd()
{
	if (MovementState == EMovementState::EMS_Blocking) return;
	if (CombatState == ECombatState::ECS_LockOn) return;

	MovementState = EMovementState::EMS_Running;
}

void APOCharacter::LockOn()
{
	if (CombatState == ECombatState::ECS_LockOn)
	{
		CombatState = ECombatState::ECS_Armed;
		if(Cast<AEnemy>(LockedOnEnemy)) Cast<AEnemy>(LockedOnEnemy)->LockedOnImage->SetVisibility(false);
		if (Cast<AEnemyBoss>(LockedOnEnemy)) Cast<AEnemyBoss>(LockedOnEnemy)->LockedOnImage->SetVisibility(false);
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
		700.f,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
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

		if (AEnemy* Enemy = Cast<AEnemy>(SphereHit.GetActor()))
		{
			LockedOnEnemy = Enemy;
			Enemy->LockedOnImage->SetVisibility(true);
			CombatState = ECombatState::ECS_LockOn;
			Enemy->HealthBarWidget->SetVisibility(true);
		}

		if (AEnemyBoss* Boss = Cast<AEnemyBoss>(SphereHit.GetActor()))
		{
			LockedOnEnemy = Boss;
			Boss->LockedOnImage->SetVisibility(true);
			CombatState = ECombatState::ECS_LockOn;
		}
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
		EDrawDebugTrace::None,
		SphereHit,
		true
	);

	if (SphereHit.GetActor())
	{
		if (LockedOnEnemy)
		{
			Cast<AEnemy>(LockedOnEnemy)->LockedOnImage->SetVisibility(false);
		}
		LockedOnEnemy = Cast<AEnemy>(SphereHit.GetActor());
		Cast<AEnemy>(LockedOnEnemy)->LockedOnImage->SetVisibility(true);
	}
}

void APOCharacter::Attack()
{
	if (MovementState == EMovementState::EMS_Blocking) return;
	if (MovementState == EMovementState::EMS_GettingHit) return;
	if (MovementState == EMovementState::EMS_Dodging) return;
	if (MovementState == EMovementState::EMS_Attacking && !CanNextCombo) return;
	if (MovementState == EMovementState::EMS_Jumping) return;

	if (CombatState == ECombatState::ECS_Unarmed)
	{
		EquipUnequip();
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (EquipMontage && AnimInstance->Montage_IsPlaying(EquipMontage))
	{
		return;
	}

	if (FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1))
	{
		CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	}
	CanNextCombo = false;


	FName SectionName = *FString::Printf(TEXT("Attack%d"), CurrentCombo);
	
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		MovementState = EMovementState::EMS_Attacking;
	}
}

void APOCharacter::Block()
{
	if (MovementState == EMovementState::EMS_Attacking) return;
	if (MovementState == EMovementState::EMS_Dodging) return;
	if (CombatState == ECombatState::ECS_Unarmed) return;

	MovementState = EMovementState::EMS_Blocking;
}

void APOCharacter::BlockEnd()
{
	if (MovementState == EMovementState::EMS_Dodging) return;
	if (CombatState == ECombatState::ECS_Unarmed) return;

	MovementState = EMovementState::EMS_Running;
}

void APOCharacter::UsePortion()
{
	if (MovementState == EMovementState::EMS_Blocking) return;
	if (MovementState == EMovementState::EMS_GettingHit) return;
	if (MovementState == EMovementState::EMS_Jumping) return;
	if (MovementState == EMovementState::EMS_Dodging) return;
	if (MovementState == EMovementState::EMS_Attacking) return;

	if (PortionNum >= 1)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && PortionMontage)
		{
			AnimInstance->Montage_Play(PortionMontage);
		}
	}
}

void APOCharacter::UsePortionEnd()
{
	PortionNum -= 1;
	CharacterInfo.CharacterStat.Health = FMath::Clamp(CharacterInfo.CharacterStat.Health + 40.f, 0.f, CharacterInfo.CharacterStat.MaxHealth);

	if (APOPlayerController* POController = Cast<APOPlayerController>(GetController()))
	{
		POController->SetHealthPercent(CharacterInfo.CharacterStat.MaxHealth, CharacterInfo.CharacterStat.Health);
		POController->SetPortionText(PortionNum);
	}
	
}

void APOCharacter::BlockBreaked()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && BlockingBreakMontage)
	{
		AnimInstance->Montage_Play(BlockingBreakMontage);
	}
	bIsBlockBreaked = true;
	GetWorldTimerManager().SetTimer(BlockResetTimer, this, &APOCharacter::BlockReset, 4.f, false);
}

void APOCharacter::BlockReset()
{
	GetWorldTimerManager().ClearTimer(BlockResetTimer);
	bIsBlockBreaked = false;
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

FVector APOCharacter::GetDesiredVelocity(FVector None)
{
	FRotator ControlRotationZ = FRotator(0.f, GetControlRotation().Yaw, 0.f);

	FVector ControlForward = UKismetMathLibrary::GetForwardVector(ControlRotationZ);
	FVector ControlRight = UKismetMathLibrary::GetRightVector(ControlRotationZ);

	FVector Velocity = \
		UKismetMathLibrary::Add_VectorVector(\
			UKismetMathLibrary::Multiply_VectorFloat(ControlForward, Input_FB), \
			UKismetMathLibrary::Multiply_VectorFloat(ControlRight, Input_RL) \
		);
	Velocity.Normalize();
	return Velocity;
}

float APOCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (APOPlayerController* POController = Cast<APOPlayerController>(GetController()))
	{
		POController->SetHealthPercent(CharacterInfo.CharacterStat.MaxHealth, CharacterInfo.CharacterStat.Health);
	}

	return DamageAmount;
}

void APOCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);

	if (MovementState == EMovementState::EMS_Dodging) return;
	if (MovementState == EMovementState::EMS_Death) return;

	if (MovementState == EMovementState::EMS_Blocking) {
		if (CharacterInfo.CharacterStat.Stamina >= 20.f)
		{
			CharacterInfo.CharacterStat.Stamina -= 20.f;
			Controller->SetStaminaPercent(CharacterInfo.CharacterStat.MaxStamina, CharacterInfo.CharacterStat.Stamina);
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && BlockingMontage)
			{
				AnimInstance->Montage_Play(BlockingMontage);
			}
		}
		else
		{
			CharacterInfo.CharacterStat.Stamina = 0.f;
			BlockBreaked();
		}
		return;
	}

	FVector Forward = GetActorForwardVector();
	FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1;
	}

	FName Section("B");

	if (-45.f <= Theta && Theta < 45.f)
	{
		Section = FName("F");
	}
	else if (-135.f <= Theta && Theta < -45.f)
	{
		Section = FName("L");
	}
	else if (45.f <= Theta && Theta < 135.f)
	{
		Section = FName("R");
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && GetHitMontage)
	{
		AnimInstance->Montage_Play(GetHitMontage);
		AnimInstance->Montage_JumpToSection(Section, GetHitMontage);
	}
}

bool APOCharacter::IsAlive_Implementation()
{
	if (MovementState == EMovementState::EMS_Death) return true;
	else return false;
}
bool APOCharacter::IsPlayer_Implementation()
{
	return true;
}

void APOCharacter::Death()
{
	if (MovementState == EMovementState::EMS_Death) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);

	MovementState = EMovementState::EMS_Death;

	if (DeathWidget)
	{
		DeathWidget->AddToViewport();
	}
}