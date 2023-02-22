// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/AIHelperInterface.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECTO_API AEnemy : public ABaseCharacter, public IAIHelperInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
	class UEnemyHealthBarWidgetComponent* HealthBarWidget;
	UPROPERTY(VisibleAnywhere)
	class ULockedOnWidgetComponent* LockedOnImage;
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/** Attack */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackBasic();
	UPROPERTY(EditAnywhere)
	class UAnimMontage* SkillMontage_1;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackSkill_1();

	/** AIHelper */
	virtual bool IsAlive_Implementation() override;
	virtual bool IsPlayer_Implementation() override;

	/** AI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanPatrol = true;
};
