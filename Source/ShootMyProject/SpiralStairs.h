// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpiralStairs.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class SHOOTMYPROJECT_API ASpiralStairs : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASpiralStairs();

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "1"))
	int32 StepsHeight = 20;

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "-2000.0", ClampMax = "2000.0"))
	float Radius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "3"))
	int32 StepsPerTurn = 12;

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "1"))
	int32 NumberOfTurns = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair Settings")
	FVector StepScale = FVector(1.0f, 1.0f, 0.25f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair Settings")
	FRotator StepRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inner Column Settings")
	bool bAutoInnerColumnRadius = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inner Column Settings", meta = (ClampMin = "0.0"))
	float AutoInnerColumnMargin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inner Column Settings", meta = (ClampMin = "0.0", ClampMax = "2000.0"))
	float InnerColumnRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inner Column Settings", meta = (ClampMin = "0.0"))
	float InnerColumnClearance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inner Column Settings")
	float InnerColumnHeightPadding = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> StepsISM = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> InnerColumn = nullptr;

protected:
	
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:

	void Rebuild();

	static void FitMeshToRadiusAndHeight(
		UStaticMeshComponent* Comp,
		float TargetRadiusUU,
		float TargetHeightUU
	);
};
