// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EnemyHealthBarWidgetComponent.h"
#include "Components/ProgressBar.h"

#include "HUD/EnemyHealthBar.h"

void UEnemyHealthBarWidgetComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UEnemyHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}