// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/TextViewWidget.h"
#include "Components/TextBlock.h"


void UTextViewWidget::SetTextView(FText text)
{
	TextView->SetText(text);
}