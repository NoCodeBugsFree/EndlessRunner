// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "Magnet.h"
#include "EndlessRunnerCharacter.h"

void AMagnet::OnItemMeshOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			EndlessRunnerCharacter->ActivateMagneto();
			SetLifeSpan(0.01f);
		}
	}
}
