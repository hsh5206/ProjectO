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
	UPROPERTY(BlueprintReadOnly)
	class UMainWidget* MainWidget;
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<class UMainWidget> MainWidgetClass;

	void SetStaminaPercent(float Max, float Cur);
	void SetHealthPercent(float Max, float Cur);
	void SetPortionText(int32 Num);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
