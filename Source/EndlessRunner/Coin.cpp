// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "Coin.h"
#include "EndlessRunnerCharacter.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetSphereRadius(10000.f);
	InteractionSphere->SetupAttachment(RootComponent);
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnInteractionSphereOverlapBegin);
}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move coin to Magneto
	if (EndlessRunnerCharacterReference && EndlessRunnerCharacterReference->IsMagneto())
	{
		FVector CurrentLocation = GetActorLocation();
		FVector DesiredLocation = EndlessRunnerCharacterReference->GetActorLocation();

		FVector ThisFrameDisplacement = FMath::VInterpTo(CurrentLocation, DesiredLocation, DeltaTime, InterpSpeed);
		SetActorLocation(ThisFrameDisplacement);

		/** Increase InterpSpeed per tick for smooth acceleration */
		InterpSpeed *= IncreaseInterpSpeed;
	}
}

void ACoin::OnItemMeshOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		if (bIsActive)
		{
			// deactivate actor
			bIsActive = false;
		
			AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
			if (EndlessRunnerCharacter)
			{
				EndlessRunnerCharacter->AddCoin();
		
				// disabling collision for the whole actor
				SetActorEnableCollision(false);
		
				if (InteractionSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, InteractionSound, GetActorLocation());
				}
		
				SetLifeSpan(0.001f);
			}
		}
	}
}

void ACoin::OnInteractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			EndlessRunnerCharacterReference = EndlessRunnerCharacter;
		}
	}
}
