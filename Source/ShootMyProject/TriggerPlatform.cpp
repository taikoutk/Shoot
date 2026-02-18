// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerPlatform.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "Components/StaticMeshComponent.h"

ATriggerPlatform::ATriggerPlatform()
{
 	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetNetUpdateFrequency(30.0f);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(PlatformMesh);

	TriggerBox->SetupAttachment(RootComponent);
}

//
void ATriggerPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATriggerPlatform, bIsActivated);

	DOREPLIFETIME(ATriggerPlatform, MovePointA);
	DOREPLIFETIME(ATriggerPlatform, MovePointB);
	DOREPLIFETIME(ATriggerPlatform, bMoveEnabled);
	DOREPLIFETIME(ATriggerPlatform, MoveStartTimeServer);
	DOREPLIFETIME(ATriggerPlatform, bMoveToB);
}

void ATriggerPlatform::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerPlatform::OnTriggerOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerPlatform::OnTriggerOverlapEnd);

	this->OnTriggerActivated.AddDynamic( this, &ATriggerPlatform::OnPlatformTriggerActivated );
	
	if (HasAuthority())
	{
		MovePointA = GetActorLocation();
		MovePointB = MovePointA + FVector(300.f, 0.f, 0.f);
		MoveStartTimeServer = GetWorld()->GetTimeSeconds();
		bMoveToB = true;
	}

	LocalStart = GetActorLocation();
	LocalTarget = GetActorLocation();
}

void ATriggerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bMoveEnabled)
		return;

	if (HasAuthority())
	{
		const FVector Dest = bMoveToB ? MovePointB : MovePointA;
		const FVector NewLoc = FMath::VInterpConstantTo(GetActorLocation(), Dest, DeltaTime, MoveSpeed);
		SetActorLocation(NewLoc);

		if (FVector::DistSquared(NewLoc, Dest) < FMath::Square(2.f))
		{
			bMoveToB = !bMoveToB;
			MoveStartTimeServer = GetWorld()->GetTimeSeconds();
			Multicast_SyncMoveState();
		}

		return;
	}

	{
		const FVector Dest = bMoveToB ? MovePointB : MovePointA;

		const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), Dest, DeltaTime, 6.f);
		SetActorLocation(NewLoc);
	}
}

void ATriggerPlatform::OnRep_MoveEnabled()
{
	LocalStart = GetActorLocation();
	LocalTarget = GetActorLocation();
}

//
void ATriggerPlatform::OnPlatformTriggerActivated(bool bIsActivated_In)
{
	if (bIsActivated_In)
	{
		// blueprint: change material/color to active
	}
	else
	{
		// blueprint: returs its color
	}

}

//
void ATriggerPlatform::Server_SetIsActivated_Implementation(bool bIsActivated_In)
{
	Multicast_SetIsActivated(bIsActivated_In);

}

//
void ATriggerPlatform::Multicast_SetIsActivated_Implementation(bool bIsActivated_In)
{
	SetIsActivated(bIsActivated_In);
}

//
void ATriggerPlatform::Client_SetIsActivated_Implementation(bool bIsActivated_In)
{
	SetIsActivated(bIsActivated_In);

}

void ATriggerPlatform::Multicast_SyncMoveState_Implementation()
{
	LocalStart = GetActorLocation();
	LocalTarget = bMoveToB ? MovePointB : MovePointA;
}

//
void ATriggerPlatform::OnRep_IsActivated()
{
	if (OnTriggerActivated.IsBound())
	{
		OnTriggerActivated.Broadcast(bIsActivated);
	}
}

//
void ATriggerPlatform::SetIsActivated(bool bIsActivated_In)
{
	bIsActivated = bIsActivated_In;

	if (OnTriggerActivated.IsBound())
	{
		OnTriggerActivated.Broadcast(bIsActivated);

	}

}

//
void ATriggerPlatform::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	APawn* OtherPawn = Cast<APawn>( OtherActor );
	if (!IsValid(OtherPawn))
		return;

	if (HasAuthority())
	{
		OverlappedPawns.AddUnique(OtherPawn);

		if (!bIsActivated && OverlappedPawns.Num() > 0)
		{
			Multicast_SetIsActivated(true);
		}
	}

}

//
void ATriggerPlatform::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* OtherPawn = Cast<APawn>(OtherActor);
	if (!IsValid(OtherPawn))
		return;

	OverlappedPawns.RemoveSingleSwap(OtherPawn);

	if (HasAuthority())
	{
		if (bIsActivated && OverlappedPawns.Num() == 0)
		{
			Multicast_SetIsActivated(false);
		}
	}

}

