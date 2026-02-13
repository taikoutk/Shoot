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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StepsISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StepsISM"));
	StepsISM->SetupAttachment(Root);
	StepsISM->SetMobility(EComponentMobility::Static);
	StepsISM->SetFlags(RF_Transactional);

	InnerColumn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerColumn"));
	InnerColumn->SetupAttachment(Root);
	InnerColumn->SetMobility(EComponentMobility::Static);
	InnerColumn->SetFlags(RF_Transactional);
}

void ASpiralStairs::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Rebuild();
}

#if WITH_EDITOR
void ASpiralStairs::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Rebuild();
}
#endif

void ASpiralStairs::Rebuild()
{
	if (!StepsISM) return;

	StepsISM->ClearInstances();

	UStaticMesh* UsedStepMesh = StepsISM->GetStaticMesh();
	if (!UsedStepMesh)
	{
		return;
	}

	const float StepHalfWidth = GetStepHalfRadialSize(UsedStepMesh, StepScale);

	const int32 lTotalSteps = NumberOfTurns * StepsPerTurn;
	const float lAngleStep = 360.0f / StepsPerTurn; // deg
	const float PlacementRadius = FMath::Max(0.f, Radius - StepHalfWidth); //rad

	StepsISM->PreAllocateInstancesMemory(lTotalSteps);

	for (int32 i = 0; i < lTotalSteps; i++)
	{
		const float lAngleDeg = i * lAngleStep;
		const float lAngleRad = FMath::DegreesToRadians(lAngleDeg);

		const float lX = FMath::Cos(lAngleRad) * PlacementRadius;
		const float lY = FMath::Sin(lAngleRad) * PlacementRadius;
		const float lZ = i * StepsHeight;

		const FRotator lStepRot = FRotator(0.f, lAngleDeg, 0.f) + StepRotationOffset;
		const FTransform lStepITM(lStepRot, FVector(lX, lY, lZ), StepScale);

		StepsISM->AddInstance(lStepITM, false);	
	}

	const int32 TotalSteps = FMath::Max(1, NumberOfTurns * StepsPerTurn);
	const float StairsHeight = (TotalSteps - 1) * StepsHeight;

	if (InnerColumn && InnerColumn->GetStaticMesh())
	{
		const float TargetHeight = FMath::Max(0.f, StairsHeight + InnerColumnHeightPadding);
		InnerColumn->SetRelativeLocation(FVector(0.f, 0.f, TargetHeight * 0.5f));

		float TargetRadius = 0.f;

		if (bAutoInnerColumnRadius && UsedStepMesh)
		{
			const float InnerEdgeRadius = FMath::Max(0.f, Radius - 2.f * StepHalfWidth);
			TargetRadius = FMath::Max(0.f, InnerEdgeRadius - AutoInnerColumnMargin - InnerColumnClearance);
		}
		else
		{
			TargetRadius = FMath::Max(0.f, InnerColumnRadius - InnerColumnClearance);
		}

		FitMeshToRadiusAndHeight(InnerColumn, TargetRadius, TargetHeight);
	}
}

void ASpiralStairs::FitMeshToRadiusAndHeight(UStaticMeshComponent* Comp, float TargetRadiusUU, float TargetHeightUU)
{
	if (!Comp || !Comp->GetStaticMesh())
		return;

	const FBoxSphereBounds B = Comp->GetStaticMesh()->GetBounds();
	const FVector Ext = B.BoxExtent;

	const float MeshRadius = FMath::Max(0.001f, FMath::Max(Ext.X, Ext.Y));
	const float MeshHalfHeight = FMath::Max(0.001f, Ext.Z);

	const float DesiredRadius = FMath::Max(0.f, TargetRadiusUU);
	const float DesiredHalfHeight = FMath::Max(0.f, TargetHeightUU * 0.5f);

	const float ScaleXY = DesiredRadius / MeshRadius;
	const float ScaleZ = DesiredHalfHeight / MeshHalfHeight;

	Comp->SetRelativeScale3D(FVector(ScaleXY, ScaleXY, ScaleZ));
}
