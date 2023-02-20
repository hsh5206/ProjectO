// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

UCLASS()
class PROJECTO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	static const FName Key_HomePos;
	static const FName Key_PatrolPos;
	static const FName Key_Target;
	static const FName Key_bDetected;
	static const FName Key_EnemyState;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BTAsset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBlackboardData* BBAsset;

public:
	/** AI Perception */
	class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AISightRadius = 500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AILoseSightRadius = 50.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AIFieldOfView = 90.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AISightAge = 5.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AILastSeenLocation = 900.f;

	UFUNCTION(BlueprintCallable)
	void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
};
