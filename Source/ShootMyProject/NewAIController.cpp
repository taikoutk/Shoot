// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAIController.h"
#include "ShootMyProjectAICharacter.h"

ANewAIController::ANewAIController()
{
}

void ANewAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ANewAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (auto PossessedCharacter = Cast<AShootMyProjectAICharacter>(InPawn))
	{
		RunBehaviorTree(PossessedCharacter->BehaviorTreeAsset);
	}
}