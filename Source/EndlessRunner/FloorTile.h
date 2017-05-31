// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

class UArrowComponent;
class ABlocker;
class ACoin;
class ABomb;
class AMagnet;
class AInteractableItem;

UCLASS()
class ENDLESSRUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Get transform to next attach point  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	FTransform GetAttachTransform() const;

	/**   */
	UFUNCTION()
	void OnEndTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Death again  */
	UFUNCTION()
	void OnHitWall(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Floor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Wall01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Wall02;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* EndTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* BlockerSpawnPoint01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* BlockerSpawnPoint02;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* BlockerSpawnPoint03;

	/** Spawn area  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CoinArea;

	/** Blocker type class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	UClass* BlockerType;

	/** Bomb type class  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	UClass* BombType;

	/**  Magnet type class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	UClass* MagnetType;

	/** Coin Type class  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	UClass* CoinType;
	
private:
	
	UPROPERTY(meta = (ClampMin = 0, ClampMax = 3), EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 MinCoinsToSpawn = 1;

	UPROPERTY(meta = (ClampMin = 3, ClampMax = 10), EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 MaxCoinsToSpawn = 10;

	UPROPERTY()
	ABlocker* BlockerReference;

	/** GC  */
	UPROPERTY()
	TArray<AActor*> Items;

	UPROPERTY()
	TArray<FTransform> SpawnPoints;

	UFUNCTION()
	void SetSpawnPoints();

	/** DEPRICATED  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnBlocker();
	
	/** Spawn Coins  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnCoins();

	/**  Spawn particular Item */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnInteractableItem(UClass* ClassToSpawn);

	
	
};
