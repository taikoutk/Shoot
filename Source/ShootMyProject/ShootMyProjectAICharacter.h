// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShootMyProjectCharacter.h"
#include "ShootMyProjectAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class SHOOTMYPROJECT_API AShootMyProjectAICharacter : public AShootMyProjectCharacter
{
	GENERATED_BODY()
	
public:

	AShootMyProjectAICharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void BeginPlay() override;

};