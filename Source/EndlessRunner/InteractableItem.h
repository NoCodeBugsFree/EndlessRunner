// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractableItem.generated.h"

UCLASS()
class ENDLESSRUNNER_API AInteractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableItem();

	virtual void BeginPlay() override;

 	/** pickup interaction  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
 	void OnItemMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	virtual void OnItemMeshOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
protected:

	/** can this actor interact or not ?   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	uint32 bIsActive : 1;

	/** Interaction Sound to play  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USoundBase* InteractionSound;

	/** Visual mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;

	
	
	/** Item Rotating Movement  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class URotatingMovementComponent* RotatingMovement;
		
};
