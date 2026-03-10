// Fill out your copyright notice in the Description page of Project Settings.

#include "BarrelComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"

// Sets default values
UBarrelComponent::UBarrelComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

}

void UBarrelComponent::Fire(
	TSubclassOf<AActor> ProjectileClass,
	USceneComponent* MuzzlePoint,
	AActor* OwnerActor)
{
	if (!bCanFire) return;
	if (!ProjectileClass) return;
	if (!MuzzlePoint) return;
	if (!OwnerActor) return;

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector SpawnLocation = MuzzlePoint->GetComponentLocation();
	const FRotator SpawnRotation = MuzzlePoint->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = Cast<APawn>(OwnerActor);

	World->SpawnActor<AActor>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	bCanFire = false;
	World->GetTimerManager().SetTimer(
		FireRateTimer,
		this,
		&UBarrelComponent::ResetFire,
		FireRate,
		false
	);
}

void UBarrelComponent::ResetFire()
{
	bCanFire = true;
}


