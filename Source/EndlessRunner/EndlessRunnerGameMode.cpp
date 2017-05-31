// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "EndlessRunner.h"
#include "EndlessRunnerGameMode.h"
#include "EndlessRunnerCharacter.h"
#include "FloorTile.h"
#include "Blueprint/UserWidget.h"

AEndlessRunnerGameMode::AEndlessRunnerGameMode()
{
	
}

void AEndlessRunnerGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	for (uint8 Count = 0; Count < InitialTilesNumber; Count++)
	{
		AddFloorTile();
	}

	CreateHUD();
}

void AEndlessRunnerGameMode::AddFloorTile()
{
	if (FloorTiles.Num() > 0)
	{
		// spawn MinForwardTiles   then spawn random
		if (CurrentTilesForward != MinForwardTiles)
		{
			AddForwardFloorTile();
		}
		else
		{
			AddRandomFloorTile();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Set up floor tiles Array in GameMode BP !"));
	}
}

void AEndlessRunnerGameMode::AddForwardFloorTile()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		AFloorTile* FloorTile = World->SpawnActor<AFloorTile>(FloorTiles[0], NextSpawnpoint, SpawnParams);

		if (FloorTile)
		{
			NextSpawnpoint = FloorTile->GetAttachTransform();
			CurrentTilesForward++;
		}
	}
}

void AEndlessRunnerGameMode::AddRandomFloorTile()
{
	if (FloorTiles.Num() > 0)
	{
		// reset CurrentTilesForward for next forward generation
		CurrentTilesForward = 0;

		// spawn random tile (ramp, left, right)
		// c++ array range
		int32 RandomNumber = FMath::RandRange(0, FloorTiles.Num() - 1);

		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			AFloorTile* FloorTile = nullptr;
			
			if (FloorTiles[RandomNumber])
			{
				FloorTile = World->SpawnActor<AFloorTile>(FloorTiles[RandomNumber], NextSpawnpoint, SpawnParams);

				if (FloorTile)
				{
					NextSpawnpoint = FloorTile->GetAttachTransform();
				}
			}
			else
			{
				AddForwardFloorTile();
				UE_LOG(LogTemp, Error, TEXT("Floor Tile is not specified. Forward floor tile spawned instead. . . "));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FloorTiles not initialized!"));
	}
}

void AEndlessRunnerGameMode::CreateHUD()
{
	if (HUDWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
