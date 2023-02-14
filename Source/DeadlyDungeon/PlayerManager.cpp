// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"
#include <algorithm>
#include "HexGridManager.h"
#include "HexTile.h"
#include "GameManager.h"
#include <array>
#include "PlayerCharacter.h"

APlayerCharacter* APlayerManager::m_selectedCharacter{};
TArray<APlayerCharacter*> APlayerManager::CharacterArray{};
int APlayerManager::turnIndex{};
int APlayerManager::numberOfCharacters{};

// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	m_selectedCharacter = nullptr;
	numberOfCharacters = 0 ;
	turnIndex = 0;
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

	if (hexIndex > AHexGridManager::HexGridArray.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Index out of range, converting to highest possible"));
		hexIndex = AHexGridManager::HexGridArray.Num()-1;
	}

	if (AHexGridManager::HexGridArray[hexIndex]->getOccupied() == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Can't spawn on occupied hex"));
		return;
	}


	AHexGridManager::HexGridArray[hexIndex]->setOccupied(true);

	CharacterArray.SetNum(numberOfCharacters+1);

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
	

	FVector Location = AHexGridManager::HexGridArray[hexIndex]->getLocation();
	Location += FVector(0, 0, 7);
	APlayerCharacter* newPlayer = GetWorld()->SpawnActor<APlayerCharacter>
		(m_characterToSpawn, Location, Rotation);

	newPlayer->setHexLocation(hexIndex);
	newPlayer->setLocation(Location);
	newPlayer->setEnemy(enemy);
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
	if (m_selectedCharacter && m_selectedCharacter->getMovementLeft()>0)
	{
		
		if (!AHexGridManager::HexGridArray[destIndex]->getOccupied() && m_selectedCharacter->isIdle())
		{
			AHexGridManager::HexGridArray[m_selectedCharacter->getHexLocation()]->setOccupied(false);
			AHexGridManager::HexGridArray[destIndex]->setOccupied(true);
			AHexGridManager::highlightsOff();
			FVector destination = AHexGridManager::HexGridArray[destIndex]->getLocation() + FVector(0, 0, 7);
			m_selectedCharacter->moveToHex(destination);
			m_selectedCharacter->setHexLocation(destIndex);
		}
	}
}

void APlayerManager::occupiedHexClicked(int hexIndex)
{
	if (!m_selectedCharacter->getAttacking())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Occupied!"));
	}
}

void APlayerManager::characterClicked(APlayerCharacter& character)
{
	if (m_selectedCharacter->getAttacking())
	{
		if (AHexGridManager::validateAttack(character.getHexLocation()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Attack!"));
			m_selectedCharacter->rotateTo(character.getLocation());
			character.updateHealth(true, m_selectedCharacter->getAttack());
		}
	}
}


void APlayerManager::removeCharacter(APlayerCharacter& character)
{
	--numberOfCharacters;
	CharacterArray.RemoveAt(character.getIndex());
	setIndexes();
	checkGameOver();
}

int APlayerManager::getnextTurn()
{			
	return turnIndex;
}

void APlayerManager::checkGameOver()
{
	bool playerAlive{ false };
	bool enemyAlive{ false };

	for (APlayerCharacter* character : CharacterArray)
	{
		if (character->isEnemy())
			enemyAlive = true;
		else
			playerAlive = true;		
	}
	if (enemyAlive && playerAlive)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Game goes on!"));
	}
	else if (enemyAlive)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Enemies win!"));
		AGameManager::endGame(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Players win!"));
		AGameManager::endGame(true);
	}
}

void APlayerManager::sortByInitiative()
{
	CharacterArray.Sort();
	setIndexes();
	CharacterArray[turnIndex]->setSelectedCharacter();
} 

void APlayerManager::setIndexes()
{
	for (int i{ 0 }; i < numberOfCharacters; ++i)
	{
		CharacterArray[i]->setIndex(i);
	}
}

void APlayerManager::setNextTurn()
{
	if (!m_selectedCharacter->getMoving())
	{
		if (turnIndex < CharacterArray.Num() - 1)
		{
			CharacterArray[++turnIndex]->setSelectedCharacter();
		}
		else
		{
			turnIndex = 0;
			AGameManager::nextRound();
			CharacterArray[turnIndex]->setSelectedCharacter();
		}
	}
}

bool APlayerManager::setAttacking()
{
	if (m_selectedCharacter)
	{
		return m_selectedCharacter->setAttacking();
	}
	return false;
}

void APlayerManager::setIdle()
{
	if (m_selectedCharacter)
	{
		m_selectedCharacter->startIdling();	
	}
}
