// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"
#include "HexGridManager.h"
#include "PlayerCharacter.h"

// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PlayerArray.SetNumZeroed(maxPlayers);

}

// Called when the game starts or when spawned
void APlayerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerManager::spawnPlayer(int hexIndex)
{	
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Spawning time!"));

	FRotator Rotation = GetActorRotation();
	//FVector Location = GetActorLocation();
	
	
	FVector Location = AHexGridManager::HexGridArray[hexIndex]->getLocation();

	APlayerCharacter* newPlayer = GetWorld()->SpawnActor<APlayerCharacter>
		(m_playerCharacter, Location, Rotation);

	
	PlayerArray[numberOfPlayers++] = newPlayer;
	
}

