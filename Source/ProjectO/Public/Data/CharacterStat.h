#pragma once

#include "Engine/DataTable.h"
#include "CharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Vigor = 5; // ������
	UPROPERTY(EditAnywhere)
	int32 Endurance = 5; // ������
	UPROPERTY(EditAnywhere)
	int32 Power = 5; // ��
	UPROPERTY(EditAnywhere)
	int32 Agility = 5; // ��ø
	UPROPERTY(EditAnywhere)
	int32 Durability = 5; // ����

	UPROPERTY(VisibleAnywhere)
	int32 MaxHealth = 100 + Vigor * 5;
	UPROPERTY(VisibleAnywhere)
	int32 Health = MaxHealth;
	UPROPERTY(VisibleAnywhere)
	int32 MaxStamina = 100 + Endurance * 5;
	UPROPERTY(VisibleAnywhere)
	int32 Stamina = MaxStamina;
};