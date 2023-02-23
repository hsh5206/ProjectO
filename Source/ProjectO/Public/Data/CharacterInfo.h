#pragma once

#include "Engine/DataTable.h"
#include "CharacterStat.h"
#include "CharacterInfo.generated.h"

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName CharacterName = FName("Default");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterStat CharacterStat;
};