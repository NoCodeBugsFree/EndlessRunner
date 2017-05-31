// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "FloorTile.h"
#include "Components/ArrowComponent.h"
#include "EndlessRunnerCharacter.h"
#include "EndlessRunnerGameMode.h"
#include "Blocker.h"
#include "Coin.h"
#include "InteractableItem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(RootComponent);
	
	Wall01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall01"));
	Wall01->SetupAttachment(RootComponent);
	Wall01->SetNotifyRigidBodyCollision(true);
	

	Wall02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall02"));
	Wall02->SetupAttachment(RootComponent);
	Wall02->SetNotifyRigidBodyCollision(true);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(RootComponent);

	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndTrigger"));
	EndTrigger->SetupAttachment(RootComponent);

	BlockerSpawnPoint01 = CreateDefaultSubobject<UArrowComponent>(TEXT("BlockerSpawnPoint01"));
	BlockerSpawnPoint01->SetupAttachment(RootComponent);

	BlockerSpawnPoint02 = CreateDefaultSubobject <UArrowComponent>(TEXT("BlockerSpawnPoint02"));
	BlockerSpawnPoint02->SetupAttachment(RootComponent);

	BlockerSpawnPoint03 = CreateDefaultSubobject <UArrowComponent>(TEXT("BlockerSpawnPoint03"));
	BlockerSpawnPoint03->SetupAttachment(RootComponent);

	CoinArea = CreateDefaultSubobject<UBoxComponent>(TEXT("CoinArea"));
	CoinArea->SetupAttachment(RootComponent);
	CoinArea->SetCollisionResponseToAllChannels(ECR_Ignore);

}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	// kill character
	Wall01->OnComponentHit.AddDynamic(this, &AFloorTile::OnHitWall);
	Wall02->OnComponentHit.AddDynamic(this, &AFloorTile::OnHitWall);
	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnEndTriggerOverlapBegin);
}

void AFloorTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetSpawnPoints();

	int32 RandomInt = FMath::RandRange(1, 10);

	switch (RandomInt)
	{
	case 1: 
	case 2:
	case 3:
		// not generic function
		SpawnBlocker();
		break;

	case 4:
		SpawnInteractableItem(MagnetType);
		break;

	case 5:
		SpawnInteractableItem(BombType);
		break;

	default:

		int32 RandomIntCoinsNumber = FMath::RandRange(MinCoinsToSpawn, MaxCoinsToSpawn);
		for (int32 Count = 0; Count < RandomIntCoinsNumber; Count++)
		{
			SpawnInteractableItem(CoinType);
		}

	}
}

FTransform AFloorTile::GetAttachTransform() const
{
	return AttachPoint->GetComponentTransform();
}

// Called every frame
void AFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorTile::OnHitWall(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			EndlessRunnerCharacter->Death(Hit);
		}
	}
}

void AFloorTile::SetSpawnPoints()
{
	SpawnPoints.Add(BlockerSpawnPoint01->GetRelativeTransform());
	SpawnPoints.Add(BlockerSpawnPoint02->GetRelativeTransform());
	SpawnPoints.Add(BlockerSpawnPoint03->GetRelativeTransform());
}

void AFloorTile::SpawnBlocker()
{
	// Spawn Actor
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		
		FTransform SpawnTransform = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
		
		ABlocker* SpawnedBlocker = World->SpawnActor<ABlocker>(BlockerType, SpawnTransform, SpawnParams);
		if (SpawnedBlocker)
		{
			SpawnedBlocker->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			BlockerReference = SpawnedBlocker;
		}
	}
}

void AFloorTile::SpawnCoins()
{
	int32 CoinsToSpawn = FMath::RandRange(MinCoinsToSpawn, MaxCoinsToSpawn);
	for (int32 Count = 0; Count < CoinsToSpawn; Count++)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
		
			FVector Origin = CoinArea->Bounds.Origin;
			FVector Extent = CoinArea->Bounds.BoxExtent;
			FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

			FRotator SpawnRotation = FRotator::ZeroRotator;
			
			ACoin* SpawnedItem = World->SpawnActor<ACoin>(CoinType, SpawnLocation, SpawnRotation, SpawnParams);
			if (SpawnedItem)
			{
				Items.AddUnique(SpawnedItem);
			}
		}
	}
}

void AFloorTile::SpawnInteractableItem(UClass* ClassToSpawn)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector Origin = CoinArea->Bounds.Origin;
		FVector Extent = CoinArea->Bounds.BoxExtent;
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (ClassToSpawn)
		{
			AActor* SpawnedItem = World->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			if (SpawnedItem)
			{
				Items.AddUnique(SpawnedItem);
			}
		}
	}
}

void AFloorTile::OnEndTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			
			AEndlessRunnerGameMode* EndlessRunnerGameMode = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(this));
			if (EndlessRunnerGameMode)
			{
				// Spawn new FloorTile
				EndlessRunnerGameMode->AddFloorTile();

				// Destroy self
				SetLifeSpan(3.f);

				// Destroy children
				if (BlockerReference)
				{
					BlockerReference->SetLifeSpan(3.f);
				}

				for (AActor* Item : Items)
				{
					if (Item && !Item->IsPendingKill())
					{
						Item->SetLifeSpan(3.f);
					}
				}
			}
		}
	}
}