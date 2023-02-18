// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	/*Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(GetHealthPercentage());
	}
	if (Health == 0.f)
	{
		SetEnemyState(EEnemyState::EES_Dead);
		Dead();
	}*/
	return DamageAmount;
}
