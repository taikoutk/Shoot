// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NewAIController.generated.h"


UCLASS()
class SHOOTMYPROJECT_API ANewAIController : public AAIController
{
	GENERATED_BODY()
	
public: 

	ANewAIController();

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess( APawn* InPawn ) override;

};
