// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyBoss.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTO_API AEnemyBoss : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyBoss();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BTAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBlackboardData* BBAsset;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	class ULockedOnWidgetComponent* LockedOnImage;


/** Weapon */
public:
	UPROPERTY(VisibleAnywhere)
	float Damage = 20;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> IgnoreActors;

/** Attack */
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackBasic();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackSkill_1();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackSkill_2();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackSkill_3();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Grab();
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackBasicMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* SkillMontage1;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* SkillMontage2;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* SkillMontage3;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* GrabMontage;

	void AttackCheck(float AttackRadius);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAttackChecking = false;
};
