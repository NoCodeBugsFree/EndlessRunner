// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "CornerBase.h"
#include "EndlessRunnerCharacter.h"

ACornerBase::ACornerBase()
{
	TurnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnBox"));
	TurnBox->SetupAttachment(RootComponent);
}

void ACornerBase::BeginPlay()
{
	Super::BeginPlay();
	TurnBox->OnComponentBeginOverlap.AddDynamic(this, &ACornerBase::OnTurnBoxOverlapBegin);
	TurnBox->OnComponentEndOverlap.AddDynamic(this, &ACornerBase::OnTurnBoxOverlapEnd);
}

void ACornerBase::OnConstruction(const FTransform& Transform)
{
	// empty
	// no blockers
	// no coins
}

void ACornerBase::OnTurnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			EndlessRunnerCharacter->SetCanTurn(true);
		}
	}
}

void ACornerBase::OnTurnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			EndlessRunnerCharacter->SetCanTurn(false);
		}
	}
}
