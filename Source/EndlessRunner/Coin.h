// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableItem.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ACoin : public AInteractableItem
{
	GENERATED_BODY()

public:

	ACoin();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** pickup interaction  */
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void OnItemMeshOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	/** Add a character reference when overlap   */
	UFUNCTION()
	void OnInteractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
private:
	
	float InterpSpeed = 1.f;

	/** Increase interp speed ever tick */
	UPROPERTY(meta = (ClampMin = 1.05, ClampMax = 1.5), EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float IncreaseInterpSpeed = 1.2f;

	/** Character reference  */
	UPROPERTY()
	class AEndlessRunnerCharacter* EndlessRunnerCharacterReference;
	
	/** Interaction sphere to set the approaching character reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractionSphere;

};
