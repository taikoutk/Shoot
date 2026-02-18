// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerPlatform.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerBoxActivatedSignature, bool, bIsActivated);

UCLASS()
class SHOOTMYPROJECT_API ATriggerPlatform : public AActor
{
	GENERATED_BODY()
	
	/** METHODS **/

public:

	ATriggerPlatform();

	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void SetIsActivated(bool bIsActivated_In);

	UFUNCTION()
	void OnRep_IsActivated();

	UFUNCTION(NetMulticast, Reliable) // Server -> All clients
		void Multicast_SetIsActivated(bool bIsActivated_In);

	UFUNCTION(Server, Reliable) // Server - AutonomousProxy // Client (Char) -> Server
		void Server_SetIsActivated(bool bIsActivated_In);

	UFUNCTION(Client, Reliable) // Server -> One client
		void Client_SetIsActivated(bool bIsActivated_In);


protected:

	virtual void BeginPlay() override;


private:

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnPlatformTriggerActivated(bool bIsActivated_In);

	/** PROPERTIES **/

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintAssignable)
	FOnTriggerBoxActivatedSignature OnTriggerActivated;

protected:

	UPROPERTY( ReplicatedUsing = OnRep_IsActivated )
	bool bIsActivated = false;

	//MOVEMENT

	UFUNCTION()
	void OnRep_MoveEnabled();

	//a - start b - target
	UPROPERTY(Replicated)
	FVector MovePointA;

	UPROPERTY(Replicated)
	FVector MovePointB;

	UPROPERTY(EditAnywhere, Category = "Move")
	float MoveSpeed = 150.f;

	UPROPERTY(ReplicatedUsing = OnRep_MoveEnabled)
	bool bMoveEnabled = true;

	UPROPERTY(Replicated)
	float MoveStartTimeServer = 0.f;

	UPROPERTY(Replicated)
	bool bMoveToB = true;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SyncMoveState();

private:

	TArray<APawn*> OverlappedPawns;

	//LOCAL

	FVector LocalStart;
	FVector LocalTarget;
};
