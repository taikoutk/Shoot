// Fill out your copyright notice in the Description page of Project Settings.

#include "SpiralStairs.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"

static float GetStepHalfRadialSize(const UStaticMesh* Mesh, const FVector& StepScale)
{
	if (!Mesh) return 0.f;

	const FBoxSphereBounds B = Mesh->GetBounds();
	const FVector Ext = B.BoxExtent;

	if (Ext.X <= Ext.Y)
	{
		return Ext.X * StepScale.X;
	}
	else
	{
		return Ext.Y * StepScale.Y;
	}
}

ASpiralStairs::ASpiralStairs()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITOR
	bRunConstructionScriptOnDrag = false;
#endif

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetMobility(EComponentMobility::Movable);

	StepsISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StepsISM"));
	StepsISM->SetupAttachment(Root);
	StepsISM->SetMobility(EComponentMobility::Movable);

	StepsISM->SetFlags(RF_Transactional);

	InnerColumn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerColumn"));
	InnerColumn->SetupAttachment(Root);
	InnerColumn->SetMobility(EComponentMobility::Movable);

	InnerColumn->SetFlags(RF_Transactional);
}

void ASpiralStairs::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Rebuild();
}

void ASpiralStairs::Rebuild()
{
	if (!StepsISM)
		return;

	StepsISM->ClearInstances();

	UStaticMesh* UsedStepMesh = StepsISM->GetStaticMesh();

	if (!UsedStepMesh)
		return;

	if (StepsPerTurn <= 0 || NumberOfTurns <= 0 || StepsHeight <= 0.f) return;

	const float StepHalfWidth = GetStepHalfRadialSize(UsedStepMesh, StepScale);

	const int32 lTotalSteps = FMath::Max(1, NumberOfTurns * StepsPerTurn);
	const float lAngleStep = 360.0f / StepsPerTurn; // deg
	const float lPlacementRadius = FMath::Max(0.f, Radius - StepHalfWidth); //rad

	StepsISM->PreAllocateInstancesMemory(lTotalSteps);

	for (int32 i = 0; i < lTotalSteps; i++)
	{
		const float lAngleDeg = i * lAngleStep;
		const float lAngleRad = FMath::DegreesToRadians(lAngleDeg);

		const float lX = FMath::Cos(lAngleRad) * lPlacementRadius;
		const float lY = FMath::Sin(lAngleRad) * lPlacementRadius;
		const float lZ = i * StepsHeight;

		const FRotator lStepRot = FRotator(0.f, lAngleDeg, 0.f) + StepRotationOffset;
		const FTransform lStepITM(lStepRot, FVector(lX, lY, lZ), StepScale);

		StepsISM->AddInstance(lStepITM, false);	
	}

	StepsISM->UpdateBounds();
	StepsISM->MarkRenderStateDirty();

	if (!InnerColumn || !InnerColumn->GetStaticMesh())
		return;

	const float lStairsHeight = (lTotalSteps - 1) * StepsHeight;
	const float lTargetHeight = FMath::Max(0.f, lStairsHeight + InnerColumnHeightPadding);

	InnerColumn->SetRelativeLocation(FVector(0.f, 0.f, lTargetHeight * 0.5f));

	float lTargetRadius = 0.f;

	if (bAutoInnerColumnRadius && UsedStepMesh)
	{
			const float lInnerEdgeRadius = FMath::Max(0.f, Radius - 2.f * StepHalfWidth);
			lTargetRadius = FMath::Max(0.f, lInnerEdgeRadius - AutoInnerColumnMargin - InnerColumnClearance);
	}
	else
	{
			lTargetRadius = FMath::Max(0.f, InnerColumnRadius - InnerColumnClearance);
	}

	FitMeshToRadiusAndHeight(InnerColumn, lTargetRadius, lTargetHeight);
	
}

void ASpiralStairs::FitMeshToRadiusAndHeight(UStaticMeshComponent* Comp, float TargetRadiusUU, float TargetHeightUU)
{
	if (!Comp || !Comp->GetStaticMesh())
		return;

	const FBoxSphereBounds B = Comp->GetStaticMesh()->GetBounds();
	const FVector Ext = B.BoxExtent;

	const float lMeshRadius = FMath::Max(0.001f, FMath::Max(Ext.X, Ext.Y));
	const float lMeshHalfHeight = FMath::Max(0.001f, Ext.Z);

	const float lDesiredRadius = FMath::Max(0.f, TargetRadiusUU);
	const float lDesiredHalfHeight = FMath::Max(0.f, TargetHeightUU * 0.5f);

	const float lScaleXY = lDesiredRadius / lMeshRadius;
	const float lScaleZ = lDesiredHalfHeight / lMeshHalfHeight;

	Comp->SetRelativeScale3D(FVector(lScaleXY, lScaleXY, lScaleZ));
}
