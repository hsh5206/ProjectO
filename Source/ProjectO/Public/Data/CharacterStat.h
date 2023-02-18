#pragma once

#include "Engine/DataTable.h"
#include "CharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Vigor = 5; // »ý¸í·Â
	UPROPERTY(EditAnywhere)
	int32 Endurance = 5; // Áö±¸·Â
	UPROPERTY(EditAnywhere)
	int32 Power = 5; // Èû
	UPROPERTY(EditAnywhere)
	int32 Agility = 5; // ¹ÎÃ¸
	UPROPERTY(EditAnywhere)
	int32 Durability = 5; // ³»±¸

	UPROPERTY(VisibleAnywhere)
	int32 MaxHealth = 100 + Vigor * 5;
	UPROPERTY(VisibleAnywhere)
	int32 Health = MaxHealth;
	UPROPERTY(VisibleAnywhere)
	int32 MaxStamina = 100 + Endurance * 5;
	UPROPERTY(VisibleAnywhere)
	int32 Stamina = MaxStamina;
};