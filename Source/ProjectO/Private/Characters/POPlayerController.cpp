// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/POPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "HUD/MainWidget.h"
#include "Characters/POCharacter.h"

APOPlayerController::APOPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainWidget> widgetClass(TEXT("/Game/Blueprints/HUD/BP_MainWidget.BP_MainWidget_C"));
	if (widgetClass.Succeeded())
	{
		MainWidgetClass = widgetClass.Class;
	}
}

void APOPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APOPlayerController::SetStaminaPercent(float Max, float Cur)
{
	if (MainWidget)
	{
		MainWidget->StaminaBar->SetPercent(Cur / Max);
	}
}

void APOPlayerController::SetHealthPercent(float Max, float Cur)
{
	if (MainWidget)
	{
		MainWidget->HealthBar->SetPercent(Cur / Max);
		SetHealthString(Max, Cur);
	}
}

void APOPlayerController::SetHealthString(float Max, float Cur)
{
	if (MainWidget)
	{
		MainWidget->HealthString->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), int(Cur), int(Max))));
	}
}

void APOPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UMainWidget>(this, MainWidgetClass);
		MainWidget->AddToViewport();
		APOCharacter* POCharacter = Cast<APOCharacter>(GetPawn());
		if (POCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("hhh"));
			SetHealthString(POCharacter->CharacterInfo.CharacterStat.MaxHealth, POCharacter->CharacterInfo.CharacterStat.Health);
		}
	}
}

void APOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}