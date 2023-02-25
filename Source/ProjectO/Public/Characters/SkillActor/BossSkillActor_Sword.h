// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSkillActor_Sword.generated.h"

UCLASS()
class PROJECTO_API ABossSkillActor_Sword : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossSkillActor_Sword();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultRoot = nullptr;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;

protected:
	virtual void BeginPlay() override;

public:
	float RunningTime = 0.f;
	bool bFlying = false;
	void Spawned();
	void Rotate();
	void SKillAttack1();
};
