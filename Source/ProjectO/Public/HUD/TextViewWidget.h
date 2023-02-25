// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextViewWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTO_API UTextViewWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextView;

	void SetTextView(FText text);
};
