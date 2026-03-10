// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BarrelComponent.generated.h"

class AActor;
class USceneComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTMYPROJECT_API UBarrelComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBarrelComponent();

	void Fire(
		TSubclassOf<AActor> ProjectileClass,
		USceneComponent* MuzzlePoint,
		AActor* OwnerActor
	);

protected:

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float FireRate = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bCanFire = true;

	void ResetFire();

	FTimerHandle FireRateTimer;
};
