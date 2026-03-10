// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon.h"
#include "TimerManager.h"
#include "Engine/World.h"


ATurret::ATurret()
{
 	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBaseMesh"));
	TurretBaseMesh->SetupAttachment(SceneRoot);

	WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAttachPoint"));
	WeaponAttachPoint->SetupAttachment(TurretBaseMesh);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	if (!WeaponClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
	if (!CurrentWeapon) return;

	CurrentWeapon->AttachToComponent(
		WeaponAttachPoint,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale
	);

	//auto-fire
	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&ATurret::Fire,
		FireInterval,
		true
	);
}

void ATurret::Fire()
{
	if (!CurrentWeapon) return;

	CurrentWeapon->Fire();
}
