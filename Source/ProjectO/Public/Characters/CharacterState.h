#pragma once

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Running UMETA(DisplayName = "Running"),
	EMS_Jumping UMETA(DisplayName = "Jumping"),
	EMS_Dodging UMETA(Displayname = "Dodging")
};
