// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "EndlessRunnerCharacter.generated.h"

UCLASS(config=Game)
class AEndlessRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	AEndlessRunnerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Death it is  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Death(FHitResult Hit);
	
	/** Returns current coins number  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	int32 GetCoins() const { return Coins; }

	/** Returns passed distance  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	float GetDistance() const { return Distance; }

	/** Add one Coin  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void AddCoin() { Coins++; }

	/** Enable/disable turn ability   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetCanTurn(bool NewCanTurn) { bCanTurn = NewCanTurn; }

	/** Returns can character attract coins  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsMagneto() const { return bIsMagneto; }

	/** Returns can character break blockers  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsBobmer() const { return bIsBomber; }

	/** Activate Magneto state */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ActivateMagneto();

	/** Deactivate Magneto state */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void DeactivateMagneto();

	/** Activate Bomber state */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ActivateBomber();

	/** Deactivate Bomber state */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void DeactivateBomber();

	/** Death FX, blood, decal, sound  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnImpactFX(FHitResult Hit);

protected:

	/** Called in Tick() */
	void MoveForward();

	void MoveRight(float Value);

	/** if we should to turn now, perform it in Tick  */
	void TurnCorner(float DeltaSeconds);

	void RestartLevel();
	
	/** Character's rotation speed at the corner  */
	UPROPERTY(meta = (ClampMin = 3, ClampMax = 10), EditAnywhere, BlueprintReadWrite, Category = "AAA")
	float InterpSpeed = 10.f;
	
	/** Shows whether Character can turn now or not   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bCanTurn : 1;

	/** Shows whether Character can turn now or not   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bIsDead : 1;

	/** Shows whether Character IsMagneto now or not =) Hi, Eric!   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bIsMagneto : 1;

	/** Shows whether Character IsMagneto now or not =) Hi, Eric!   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bIsBomber : 1;

	/** Character's current desired rotation   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FRotator DesiredRotation;

	/** Character Death Blood FX  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* EmmiterTemplate;

	/** Character Death Sound   */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USoundBase* DeathSound;

	/** Restart Level timer  */
	FTimerHandle RestartTimer;
	
	/** Total COins  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 Coins = 0;

	/**Timer to activate  Magneto ability*/
	FTimerHandle MagnetoTimer;

	/** Magneto time in seconds  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float MagnetoTime = 5.f;

	/**Timer to activate  Magneto ability*/
	FTimerHandle BomberTimer;

	/** Magneto time in seconds  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float BomberTime = 5.f;

	/** Blood Material */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DecalMaterial;

	/** Blood Decal Size */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FVector DecalSize = FVector(20.f, 20.f, 20.f);

	/** Magneto Particles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MagnetoParticles;

	/** Bomber Particles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* BomberParticles;

	/** Bomber Particles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Distance = 0.f;

	/** Calculate Distance  */
	void CalcDistance(float DeltaSeconds);

	/** Dynamic Material reference  */
	UPROPERTY(Meta = (BlueprintProtected = "true"), VisibleAnywhere, BlueprintReadWrite, Category = "AAA")
	UMaterialInstanceDynamic* EndlessRunnerMaterial;

	/** Color in Magneto state */
	UPROPERTY(Meta = (BlueprintProtected = "true"), EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FLinearColor MagnetoColor = FLinearColor(0.f, 0.091f, 1.f, 1.f);

	/**  Color in Bomber state */
	UPROPERTY(Meta = (BlueprintProtected = "true"), EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FLinearColor BomberColor = FLinearColor(1.f, 0.31f, 0.f, 1.f);
	
	/** Initial character color  */
	FLinearColor InitialColor;

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

