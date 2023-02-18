// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LockedOnWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTO_API ULockedOnWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UUserWidget* LockedOnImage;
};
