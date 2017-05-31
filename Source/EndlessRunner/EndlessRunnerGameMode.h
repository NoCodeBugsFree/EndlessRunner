// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameMode.generated.h"

class UUserWidget;

UCLASS(minimalapi)
class AEndlessRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AEndlessRunnerGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void AddFloorTile();

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void AddForwardFloorTile();

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void AddRandomFloorTile();

protected:

	UPROPERTY()
	FTransform NextSpawnpoint;

private:

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void CreateHUD();

	/** Minimum number of forward tiles  */
	UPROPERTY(meta = (ClampMin = 2, ClampMax = 10),  EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 MinForwardTiles = 5;

	/** Current Tiles Forward  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 CurrentTilesForward = 0;

	/** Array of floor tiles, first one - forward tile  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AFloorTile>> FloorTiles;

	/** Initial tiles number   */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true", ClampMin = 3, ClampMax = 50))
	uint8 InitialTilesNumber = 10;
	
	/** Widget Class  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	
	/** Current Widget  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* CurrentWidget;
	
};



