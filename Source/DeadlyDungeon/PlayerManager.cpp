// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"
#include "HexGridManager.h"
#include "HexTile.h"
#include "PlayerCharacter.h"

APlayerCharacter* APlayerManager::m_selectedCharacter{};
// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PlayerArray.SetNumZeroed(maxPlayers);
	m_selectedCharacter = nullptr;

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
	FRotator Rotation = GetActorRotation();
	//FVector Location = GetActorLocation();
	
	//Occupied
	AHexGridManager::HexGridArray[hexIndex]->setOccupied(true);

	FVector Location = AHexGridManager::HexGridArray[hexIndex]->getLocation();
	Location += FVector(0, 0, 10.0);
	APlayerCharacter* newPlayer = GetWorld()->SpawnActor<APlayerCharacter>
		(m_playerCharacter, Location, Rotation);

	newPlayer->setLocation(hexIndex);

	PlayerArray[numberOfPlayers++] = newPlayer;	
}

void APlayerManager::storeSelectedCharacter(APlayerCharacter* selectedCharacter)
{
	m_selectedCharacter = selectedCharacter;
}

APlayerCharacter* APlayerManager::getSelectedCharacer()
{
	return m_selectedCharacter;
}

void APlayerManager::movePlayerCharacter(int destIndex)
{
	if (m_selectedCharacter)
	{
		if (!AHexGridManager::HexGridArray[destIndex]->getOccupied())
		{
			AHexGridManager::HexGridArray[m_selectedCharacter->getLocation()]->setOccupied(false);
			AHexGridManager::HexGridArray[destIndex]->setOccupied(true);
			m_selectedCharacter->moveToHex(AHexGridManager::HexGridArray[destIndex]->getLocation());
			m_selectedCharacter->setLocation(destIndex);
		}
	}
}
