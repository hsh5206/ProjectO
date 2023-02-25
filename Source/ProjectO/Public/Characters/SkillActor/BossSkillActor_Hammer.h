// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BossSkillActor_Hammer.generated.h"

UCLASS()
class PROJECTO_API ABossSkillActor_Hammer : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossSkillActor_Hammer();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultRoot = nullptr;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere)
	UParticleSystem* UnderPlayerParticle;
	FTimerHandle beforeAttackTimer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UTimelineComponent* Timeline;
	UPROPERTY()
	UCurveFloat* FloatCurve;

	UFUNCTION()
	void TimelineCallback(float val);
	UFUNCTION()
	void TimelineFinishedCallback();
	void PlayTimeline();

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

public:
	class ADecalActor* Decal;
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* M_UnderPlayerDecal;

	FVector AttackLocation = FVector::ZeroVector;
	bool bRotate = false;
	bool bLaunch = false;
	void Spawn();
	UFUNCTION(BlueprintCallable)
	void Rotate();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void DestroySkill();

	bool isAttacked = false;

	UPROPERTY(VisibleAnywhere)
	float Damage = 20;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> IgnoreActors;
};
