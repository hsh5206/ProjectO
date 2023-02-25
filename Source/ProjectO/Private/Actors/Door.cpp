// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Door.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/POCharacter.h"
#include "POGameInstance.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	TriggerBox->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	Mesh->SetupAttachment(GetRootComponent());
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (InteractWidgetClass)
	{
		InteractWidget = CreateWidget<UUserWidget>(GetWorld(), InteractWidgetClass);
	}
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InsideCharacter && InsideCharacter->bInteract)
	{
		InsideCharacter->bInteract = false;
		MoveLevel();
	}
}

void ADoor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APOCharacter* Character = Cast<APOCharacter>(OtherActor))
	{
		InsideCharacter = Character;
		if (InteractWidget)
		{
			InteractWidget->AddToViewport();
		}
	}
}

void ADoor::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InsideCharacter)
	{
		InsideCharacter->bInteract = false;
		InsideCharacter = nullptr;

		if (InteractWidget)
		{
			InteractWidget->RemoveFromParent();
		}
	}
}

void ADoor::MoveLevel_Implementation()
{

}