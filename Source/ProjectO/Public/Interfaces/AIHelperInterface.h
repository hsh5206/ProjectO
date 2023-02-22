// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIHelperInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAIHelperInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTO_API IAIHelperInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool IsAlive();
	UFUNCTION(BlueprintNativeEvent)
	bool IsPlayer();
};
