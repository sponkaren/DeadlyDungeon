// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"
#include <algorithm>
#include "HexGridManager.h"
#include "HexTile.h"
#include "PlayerCharacter.h"

APlayerCharacter* APlayerManager::m_selectedCharacter{};
TArray<APlayerCharacter*> APlayerManager::CharacterArray{};
// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//CharacterArray.SetNumZeroed(maxPlayers);
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

void APlayerManager::spawnPlayer(int hexIndex, bool enemy)
{	
	if (enemy)
	{
		m_characterToSpawn = m_enemyCharacter;
	}
	else
	{
		m_characterToSpawn = m_playerCharacter;
	}

	FRotator Rotation = GetActorRotation();
	//FVector Location = GetActorLocation();
	
	//Occupied
	AHexGridManager::HexGridArray[hexIndex]->setOccupied(true);

	FVector Location = AHexGridManager::HexGridArray[hexIndex]->getLocation();
	Location += FVector(0, 0, 10.0);
	APlayerCharacter* newPlayer = GetWorld()->SpawnActor<APlayerCharacter>
		(m_characterToSpawn, Location, Rotation);

	newPlayer->setHexLocation(hexIndex);
	newPlayer->setLocation(Location);


	CharacterArray[numberOfCharacters++] = newPlayer;
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
		if (!AHexGridManager::HexGridArray[destIndex]->getOccupied() && !m_selectedCharacter->getMoving())
		{
			AHexGridManager::HexGridArray[m_selectedCharacter->getHexLocation()]->setOccupied(false);
			AHexGridManager::HexGridArray[destIndex]->setOccupied(true);
			AHexGridManager::highlightsOff();
			FVector destination = AHexGridManager::HexGridArray[destIndex]->getLocation() + FVector(0, 0, 10.0);
			m_selectedCharacter->moveToHex(destination);
			m_selectedCharacter->setHexLocation(destIndex);
		}
	}
}

bool greaterInitiative(APlayerCharacter& c1, APlayerCharacter& c2)
{
	return (c1.getInitiative() < c2.getInitiative());
}

void APlayerManager::sortByInitiative()
{
	CharacterArray.Sort();

	CharacterArray[0]->setSelectedCharacter();
}