// Fill out your copyright notice in the Description page of Project Settings.

#include "SpiralStairs.h"

ASpiralStairs::ASpiralStairs()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ASpiralStairs::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BuildStair();
}

#if WITH_EDITOR
void ASpiralStairs::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);\
	BuildStair();
}
#endif

void ASpiralStairs::BuildStair()
{
	DestroyStairComponents();

	if (!StepMesh || StepsPerTurn <= 0 || NumberOfTurns <= 0)
		return;

	const int32 lTotalSteps = NumberOfTurns * StepsPerTurn; // 36
	const float lAngleStep = 360.0f / StepsPerTurn;

	for (int32 i = 0; i < lTotalSteps; i++)
	{
		FString lName = FString::Printf(TEXT("Step_%d"), i);
		UStaticMeshComponent* lStep = NewObject<UStaticMeshComponent>(this, *lName);

		lStep->SetStaticMesh(StepMesh);
		lStep->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		float lAngle = FMath::DegreesToRadians(i * lAngleStep);

		float lX = FMath::Cos(lAngle) * Radius;
		float lY = FMath::Sin(lAngle) * Radius;
		float lZ = i * Height;

		FVector lStepLocation = { lX, lY, lZ };

		lStep->SetRelativeLocation(lStepLocation);

		FRotator lStepRotation = { 0, i * lAngleStep, 0 };
		lStep->SetRelativeRotation(lStepRotation);

		lStep->RegisterComponent();

		StepComponent.Add(lStep);

	}

}

void ASpiralStairs::DestroyStairComponents()
{
	for (UStaticMeshComponent* lComp : StepComponent)
	{
		if (lComp)
		{
			lComp->DestroyComponent();

		}

	}
	StepComponent.Empty();
}

void ASpiralStairs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

