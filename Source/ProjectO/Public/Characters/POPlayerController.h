// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "POPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTO_API APOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
	APOPlayerController();
	UPROPERTY()
	class UMainWidget* MainWidget;
	UPROPERTY()
	TSubclassOf<class UMainWidget> MainWidgetClass;

	void SetStaminaPercent(float Max, float Cur);
	void SetHealthPercent(float Max, float Cur);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
