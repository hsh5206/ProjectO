// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTO_API UEnemyHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent);
private:
	UPROPERTY()
	class UEnemyHealthBar* HealthBarWidget;
};
