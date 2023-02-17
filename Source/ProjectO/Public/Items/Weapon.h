// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTO_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

protected:
	virtual void BeginPlay() override;

public:	
	void Equip(USceneComponent* InParent, const FName& InSocketName, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	UFUNCTION(BlueprintCallable)
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* WeaponBox;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceEnd;

	TArray<AActor*> IgnoreActors;

};
