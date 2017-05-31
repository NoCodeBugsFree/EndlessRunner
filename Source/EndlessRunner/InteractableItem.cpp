// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "InteractableItem.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "EndlessRunnerCharacter.h"


// Sets default values
AInteractableItem::AInteractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));

	bIsActive = true;
}

void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();

	ItemMesh->OnComponentBeginOverlap.AddDynamic(this, &AInteractableItem::OnItemMeshOverlapBegin);
}

void AInteractableItem::OnItemMeshOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("AInteractableItem::OnItemMeshOverlapBegin_Implementation - not suppose to see abstract class message. . ."));
}

