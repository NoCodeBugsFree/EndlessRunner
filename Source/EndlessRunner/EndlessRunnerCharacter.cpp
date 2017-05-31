// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "EndlessRunner.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "EndlessRunnerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AEndlessRunnerCharacter

AEndlessRunnerCharacter::AEndlessRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->GravityScale = 2.f;
	JumpMaxHoldTime = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 100.f);
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Particle Systems
	MagnetoParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagnetoParticles"));
	MagnetoParticles->SetAutoActivate(false);
	MagnetoParticles->SetupAttachment(RootComponent);

	BomberParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BomberParticles"));
	BomberParticles->SetAutoActivate(false);
	BomberParticles->SetupAttachment(RootComponent);


	/** Character initials  */
	
	bCanTurn = false;	// can't turn now. Turn available only at triggers
	bIsDead = false;    // alive
	bIsMagneto = false; // not Magneto
	bIsBomber = false;  // not Bomber
	
}

void AEndlessRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// save initial color
	if (EndlessRunnerMaterial)
	{
		EndlessRunnerMaterial->GetVectorParameterValue(FName("BodyColor"), InitialColor);
	}
}

void AEndlessRunnerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsDead)
	{
		MoveForward();
		TurnCorner(DeltaSeconds);
		CalcDistance(DeltaSeconds);
	}
}

void AEndlessRunnerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	EndlessRunnerMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
}

void AEndlessRunnerCharacter::Death(FHitResult Hit)
{
	// Die once
	if (!bIsDead)
	{
		// Kill Ragdoll Character
		DetachFromControllerPendingDestroy();

		SpawnImpactFX(Hit);

		if (GetMesh())
		{
			static FName CollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetCollisionProfileName(CollisionProfileName);
		}

		SetActorEnableCollision(true);


		// rag doll  (init Physics)
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		// disable movement
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);

		// disable collisions on the capsule
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

		// restart level 2.5 sec later
		GetWorldTimerManager().SetTimer(RestartTimer, this, &AEndlessRunnerCharacter::RestartLevel, 2.5f, false);
	}

	bIsDead = true;
}

void AEndlessRunnerCharacter::ActivateMagneto()
{
	bIsMagneto = true;
	MagnetoParticles->Activate(true);

	if (EndlessRunnerMaterial)
	{
		EndlessRunnerMaterial->SetVectorParameterValue(FName("BodyColor"), MagnetoColor);
	}

	GetWorldTimerManager().SetTimer(MagnetoTimer, this, &AEndlessRunnerCharacter::DeactivateMagneto, MagnetoTime, false);
}

void AEndlessRunnerCharacter::DeactivateMagneto()
{
	bIsMagneto = false;
	MagnetoParticles->Deactivate();
	if (EndlessRunnerMaterial)
	{
		EndlessRunnerMaterial->SetVectorParameterValue(FName("BodyColor"), InitialColor);
	}
}

void AEndlessRunnerCharacter::ActivateBomber()
{
	bIsBomber = true;
	BomberParticles->Activate(true);

	if (EndlessRunnerMaterial)
	{
		EndlessRunnerMaterial->SetVectorParameterValue(FName("BodyColor"), BomberColor);
	}

	GetWorldTimerManager().SetTimer(BomberTimer, this, &AEndlessRunnerCharacter::DeactivateBomber, BomberTime, false);
}

void AEndlessRunnerCharacter::DeactivateBomber()
{
	bIsBomber = false;
	BomberParticles->Deactivate();
	if (EndlessRunnerMaterial)
	{
		EndlessRunnerMaterial->SetVectorParameterValue(FName("BodyColor"), InitialColor);
	}
}

void AEndlessRunnerCharacter::SpawnImpactFX(FHitResult Hit)
{
	// spawn blood particles
	if (EmmiterTemplate)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmmiterTemplate, GetActorTransform());
	}

	// play death/impact sound cue
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	// spawn decal
	if (DecalMaterial)
	{
		UGameplayStatics::SpawnDecalAtLocation(this, DecalMaterial, DecalSize, Hit.ImpactPoint);
	}
}

void AEndlessRunnerCharacter::CalcDistance(float DeltaSeconds)
{
	float ThisFrameDistance = DeltaSeconds * GetCharacterMovement()->Velocity.Size() / 100.f;
	
	Distance += ThisFrameDistance;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEndlessRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//MoveRight
	PlayerInputComponent->BindAxis("MoveRight", this, &AEndlessRunnerCharacter::MoveRight);
}

void AEndlessRunnerCharacter::MoveForward()
{
	if ((Controller != NULL))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, 1.f);
	}
}

void AEndlessRunnerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0) && ( !bCanTurn ))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
	
	// Corner turn
	if(bCanTurn && (Controller != NULL))
	{
		if (Value < 0.f)
		{
			DesiredRotation = UKismetMathLibrary::ComposeRotators(DesiredRotation, FRotator(0.f, -90.f, 0.f));

			// one turn per trigger
			bCanTurn = false;
		}
		else if (Value > 0.f)
		{
			DesiredRotation = UKismetMathLibrary::ComposeRotators(DesiredRotation, FRotator(0.f, 90.f, 0.f));

			// one turn per trigger
			bCanTurn = false;
		}
	}
}

void AEndlessRunnerCharacter::TurnCorner(float DeltaSeconds)
{
	if ( ! GetControlRotation().Equals(DesiredRotation, 0.0001f) )
	{
		FRotator NewRotation = FMath::RInterpTo(GetControlRotation(), DesiredRotation, DeltaSeconds, InterpSpeed);
		GetController()->SetControlRotation(NewRotation);
	}
}

void AEndlessRunnerCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName("RunMap"));
}

