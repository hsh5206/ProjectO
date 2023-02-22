// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"

#include "Characters/POCharacter.h"
#include "Characters/Enemy.h"

const FName AEnemyAIController::Key_HomePos(TEXT("HomePos"));
const FName AEnemyAIController::Key_PatrolPos(TEXT("PatrolPos"));
const FName AEnemyAIController::Key_Target(TEXT("Target"));
const FName AEnemyAIController::Key_bDetected(TEXT("bDetected"));
const FName AEnemyAIController::Key_AIState(TEXT("EAIState"));
const FName AEnemyAIController::Key_bCanPatrol(TEXT("bCanPatrol"));

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

	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::PerceptionUpdated);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledPawn = InPawn;

	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(Key_HomePos, InPawn->GetActorLocation());
		RunBehaviorTree(BTAsset);
		Blackboard->SetValueAsBool(Key_bCanPatrol ,Cast<AEnemy>(InPawn)->CanPatrol);
	}
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		if (APOCharacter* POCharacter = Cast<APOCharacter>(UpdatedActor))
		{
			FActorPerceptionBlueprintInfo Info;
			if (GetAIPerceptionComponent()->GetActorsPerception(UpdatedActor, Info))
			{
				for (FAIStimulus Stim : Info.LastSensedStimuli)
				{
					if (Stim.WasSuccessfullySensed())
					{
						Cast<AEnemy>(ControlledPawn)->LockedOnEnemy = UpdatedActor;
						Blackboard->SetValueAsObject(Key_Target, UpdatedActor);
						Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::EAIS_Chase);
					}
					/*else
					{
						Blackboard->SetValueAsObject(Key_Target, nullptr);
						Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::EAIS_Passive);
					}*/
				}
			}
		}
	}
}