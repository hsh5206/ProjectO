// Fill out your copyright notice in the Description page of Project Settings.


#include "POGameInstance.h"
#include "POSaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/POCharacter.h"
#include "Characters/POPlayerController.h"

UPOGameInstance::UPOGameInstance()
{

}

void UPOGameInstance::SaveGame(float Health, float Stamina, int32 PortionNum, FVector Location, FRotator Rotation)
{
	UPOSaveGame* SG = Cast<UPOSaveGame>(UGameplayStatics::CreateSaveGameObject(UPOSaveGame::StaticClass()));
	SG->Health = Health;
	SG->Stamina = Stamina;
	SG->PortionNum = PortionNum;
	SG->Location = Location;
	SG->Rotation = Rotation;

	UGameplayStatics::SaveGameToSlot(SG, FString::Printf(TEXT("AutoSave")), 0);
}

void UPOGameInstance::LoadGame(APOCharacter* Character, FString SlotName, int32 index)
{
	UPOSaveGame* LG = Cast<UPOSaveGame>(UGameplayStatics::CreateSaveGameObject(UPOSaveGame::StaticClass()));
	LG = Cast<UPOSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, index));

	if (LG)
	{
		Character->CharacterInfo.CharacterStat.Health = LG->Health;
		Character->CharacterInfo.CharacterStat.Stamina = LG->Stamina;
		Character->PortionNum = LG->PortionNum;
		Character->SetActorLocation(LG->Location);
		Character->SetActorRotation(LG->Rotation);

		APOPlayerController* Controller = Cast<APOPlayerController>(Character->GetController());
		Controller->SetHealthPercent(Character->CharacterInfo.CharacterStat.MaxHealth, Character->CharacterInfo.CharacterStat.Health);
		Controller->SetStaminaPercent(Character->CharacterInfo.CharacterStat.MaxStamina, Character->CharacterInfo.CharacterStat.Stamina);
		Controller->SetPortionText(Character->PortionNum);
	}
}

void UPOGameInstance::LoadGameFromTrans(APOCharacter* Character, FString SlotName = FString::Printf(TEXT("AutoSave")), int32 index = 0)
{
	UPOSaveGame* LG = Cast<UPOSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, index));

	if (!LG)
	{
		LG = GetMutableDefault<UPOSaveGame>();
	}

	Character->CharacterInfo.CharacterStat.Health = LG->Health;
	Character->CharacterInfo.CharacterStat.Stamina = LG->Stamina;
	Character->PortionNum = LG->PortionNum;

	OnTransEndDelegate.Broadcast();

	UGameplayStatics::DeleteGameInSlot(SlotName, index);
}