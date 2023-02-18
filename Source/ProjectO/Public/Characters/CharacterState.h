#pragma once

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Running UMETA(DisplayName = "Running"),
	EMS_Jumping UMETA(DisplayName = "Jumping"),
	EMS_Dodging UMETA(Displayname = "Dodging"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unarmed UMETA(DisplayName = "Unarmed"),
	ECS_Armed UMETA(DisplayName = "Armed"),
	ECS_LockOn UMETA(Displayname = "LockOn")
};

struct FDodgeDirection
{
public:
	bool bFront = true;
	bool bBack = false;
	bool bRight = false;
	bool bLeft = false;
};