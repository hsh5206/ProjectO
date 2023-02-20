// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComponent = Blackboard.Get();

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/Enemy/AI/BT_Enemy.BT_Enemy"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/Enemy/AI/BB_Enemy.BB_Enemy"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		RunBehaviorTree(BTAsset);
	}
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
