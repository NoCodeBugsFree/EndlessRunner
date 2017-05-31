// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FloorTile.h"
#include "RampUp.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ARampUp : public AFloorTile
{
	GENERATED_BODY()
	
public:

	ARampUp();


protected:



private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RampMesh;
	
};
