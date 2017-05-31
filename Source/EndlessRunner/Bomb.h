// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableItem.h"
#include "Bomb.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ABomb : public AInteractableItem
{
	GENERATED_BODY()

public:
	
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void OnItemMeshOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;


protected:



private:
	
	
	
};
