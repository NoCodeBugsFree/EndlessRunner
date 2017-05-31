// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "Bomb.h"
#include "EndlessRunnerCharacter.h"

void ABomb::OnItemMeshOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			EndlessRunnerCharacter->ActivateBomber();
			SetLifeSpan(0.01f);
		}
	}
}
