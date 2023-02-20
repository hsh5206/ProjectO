// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy.h"
#include "Kismet/GameplayStatics.h"

#include "HUD/EnemyHealthBarWidgetComponent.h"
#include "HUD/LockedOnWidgetComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HealthBarWidget = CreateDefaultSubobject<UEnemyHealthBarWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	LockedOnImage = CreateDefaultSubobject<ULockedOnWidgetComponent>(TEXT("LockedOnImage"));
	LockedOnImage->SetupAttachment(GetRootComponent());
	LockedOnImage->SetVisibility(false);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthBarWidget)
	{
		float Percent = float(CharacterInfo.CharacterStat.Health) / float(CharacterInfo.CharacterStat.MaxHealth);
		HealthBarWidget->SetHealthPercent(Percent);
	}

	return DamageAmount;
}
