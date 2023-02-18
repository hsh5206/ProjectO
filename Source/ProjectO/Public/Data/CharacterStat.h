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
	float MaxHealth = 100 + Vigor * 5;
	UPROPERTY(VisibleAnywhere)
	float Health = MaxHealth;
	UPROPERTY(VisibleAnywhere)
	float MaxStamina = 100 + Endurance * 5;
	UPROPERTY(VisibleAnywhere)
	float Stamina = MaxStamina;
};