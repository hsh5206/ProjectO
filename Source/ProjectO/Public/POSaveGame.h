// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "POSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTO_API UPOSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float Health = 125.f;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float Stamina = 125.f;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 PortionNum = 3;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector Location;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator Rotation;
};
