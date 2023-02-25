// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "POGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_Dynamic);

UCLASS()
class PROJECTO_API UPOGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic OnTransEndDelegate;

public:
	UPOGameInstance();

	/** Save & Load */
	UFUNCTION(BlueprintCallable)
	void SaveGame(float Health, float Stamina, int32 PortionNum, FVector Location, FRotator Rotation);
	UFUNCTION(BlueprintCallable)
	void LoadGame(class APOCharacter* Character, FString SlotName, int32 index);
	UFUNCTION(BlueprintCallable)
	void LoadGameFromTrans(APOCharacter* Character, FString SlotName, int32 index);
};
