// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "RampUp.h"

ARampUp::ARampUp()
{
	RampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RampMesh"));
	RampMesh->SetupAttachment(RootComponent);
	
}
