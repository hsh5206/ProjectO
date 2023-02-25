// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SkillActor/BossSkillActor_Sword.h"

ABossSkillActor_Sword::ABossSkillActor_Sword()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ItemMesh->SetupAttachment(GetRootComponent());
}

void ABossSkillActor_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossSkillActor_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	float DeltaZ = 0.25f * FMath::Sin(RunningTime * 5.f);
	AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));
}

