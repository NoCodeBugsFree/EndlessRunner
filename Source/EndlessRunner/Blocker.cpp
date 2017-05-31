// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner.h"
#include "Blocker.h"
#include "EndlessRunnerCharacter.h"

// Sets default values
ABlocker::ABlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockerMesh"));
	SetRootComponent(BlockerMesh);
	
	BlockerMesh->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ABlocker::BeginPlay()
{
	Super::BeginPlay();
	
	BlockerMesh->OnComponentHit.AddDynamic(this, &ABlocker::OnHit);
}

// Called every frame
void ABlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlocker::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AEndlessRunnerCharacter* EndlessRunnerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);
		if (EndlessRunnerCharacter)
		{
			if (EndlessRunnerCharacter->IsBobmer())
			{
				//  destroy self TODO Physics
				Destroy();
			} 
			else // kill character
			{
				EndlessRunnerCharacter->Death(Hit);
			}
		}
	}
}

