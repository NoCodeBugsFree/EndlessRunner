// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FloorTile.h"
#include "CornerBase.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ACornerBase : public AFloorTile
{
	GENERATED_BODY()

public:

	ACornerBase();

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Enable character turn ability  */
	UFUNCTION()
	void OnTurnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** disable character turn ability  */
	UFUNCTION()
	void OnTurnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:



private:
	
	/** Trigger Box for setting up enable/disable character turn ability */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TurnBox;
	
};
