// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpiralStairs.generated.h"

UCLASS()
class SHOOTMYPROJECT_API ASpiralStairs : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASpiralStairs();

	UPROPERTY(EditAnywhere, Category = "Stair Settings")
	UStaticMesh* StepMesh;

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "1"))
	int32 Height = 20;

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "-2000.0", ClampMax = "2000.0"))
	float Radius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "3"))
	int32 StepsPerTurn = 12;

	UPROPERTY(EditAnywhere, Category = "Stair Settings", meta = (ClampMin = "1"))
	int32 NumberOfTurns = 3;

protected:
	
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Transient)
	TArray<UStaticMeshComponent*> StepComponent;

	void BuildStair();
	void DestroyStairComponents();

};
