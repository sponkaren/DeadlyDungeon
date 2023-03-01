// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"
#include <algorithm>
#include <array>
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LatentActionManager.h"
#include "assert.h"
#include "HexGridManager.h"
#include "HexTile.h"
#include "StatGenerator.h"
#include "PlayerCharacter.h"

// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	m_selectedCharacter = nullptr;
	numberOfCharacters = 0 ;
	turnIndex = 0;
}

void APlayerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void APlayerManager::spawnHexGridManager()
{
	FRotator Rotation{ 0,0,0 };
	FVector Location{ 0,0,0 };

	hexManager = GetWorld()->SpawnActor<AHexGridManager>(bp_hexManager, Location, Rotation);
	hexManager->thisHexClicked.AddDynamic(this, &APlayerManager::whenHexClicked);
	hexManager->setAdjacentHex();
}

void APlayerManager::handlePlayersToSpawn(TArray<FPlayerStruct>& players)
{
	
	for (FPlayerStruct stats : players)
	{
		spawnPlayer(stats, hexManager->getNextPlayerSpawn(), false);
	}
	
}

void APlayerManager::spawnEnemies(int difficulty, int numberOfEnemies)
{
	for (int i{ 0 }; i < numberOfEnemies; ++i)
	{
		FPlayerStruct stats{ StatGenerator::generateStats(difficulty) };
		spawnPlayer(stats, hexManager->getNextEnemySpawn(), true);
	}
}

void APlayerManager::spawnPlayer(FPlayerStruct& stats, int hexIndex, bool enemy)
{	
	/*
	if (hexIndex >= hexManager->HexGridArray.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Index out of range, converting to highest possible"));
		hexIndex = hexManager->HexGridArray.Num()-1;
	}

	if (hexManager->HexGridArray[hexIndex]->getOccupied() == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Can't spawn on occupied hex"));
		return;
	}

	hexManager->HexGridArray[hexIndex]->setOccupied(true);
	*/

	FRotator Rotation = GetActorRotation() + FRotator(0,30,0);

	if (enemy)
	{
		m_characterToSpawn = m_enemyCharacter;
		Rotation += FRotator(0, 180, 0);
	}
	else
	{
		m_characterToSpawn = m_playerCharacter;
	}


	FVector Location = hexManager->HexGridArray[hexIndex]->getLocation();
	Location += FVector(0, 0, 7);

	FTransform SpawnTransform(Rotation, Location);

	APlayerCharacter* newPlayer = GetWorld()->SpawnActorDeferred<APlayerCharacter>(m_characterToSpawn, SpawnTransform);
	newPlayer->Init(stats);
	newPlayer->FinishSpawning(SpawnTransform);

	CharacterArray.Emplace(newPlayer);

	//APlayerCharacter* newPlayer = CharacterArray.Emplace_GetRef(GetWorld()->SpawnActor<APlayerCharacter>
		//(m_characterToSpawn, Location, Rotation));

	++numberOfCharacters;

	newPlayer->setHexLocation(hexIndex);
	newPlayer->setLocation(Location);
	newPlayer->setEnemy(enemy);

	newPlayer->CharClicked.AddDynamic(this, &APlayerManager::characterClicked);
	newPlayer->CharIdle.AddDynamic(this, &APlayerManager::setIdle);
}

void APlayerManager::storeSelectedCharacter(APlayerCharacter* selectedCharacter)
{
	m_selectedCharacter = selectedCharacter;
}

void APlayerManager::setSelectedCharacter(APlayerCharacter* character)
{
	if (IsValid(lastClicked))
	{
		lastClicked->setMovementLeft(0);
		setIdle(lastClicked);
		lastClicked->setArrowOn(false);
		hexManager->highlightsOff();
	}

	lastClicked = character;
	character->setArrowOn(true);
	character->resetMoveAtk();
	characterMovement.Empty();
	character->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character->m_selectedAnim, true);
	APlayerManager::storeSelectedCharacter(character);
	hexManager->highlightTiles(character->getHexLocation(),m_selectedCharacter->m_movementLeft);	
	
	if (character->m_type == CharacterType::ENEMY)
	{
		startAI();
	}	
}

void APlayerManager::startAI()
{
	int characterHex{m_selectedCharacter->getHexLocation() };
	//characterMovement.Empty();
	TArray<int> targetLocations;

	for (APlayerCharacter* possibleTarget : CharacterArray)
	{
		if (possibleTarget->m_type == CharacterType::ALLY)
		{
			targetLocations.Emplace(possibleTarget->getHexLocation());
		}
	}

	if (targetLocations.Num() == 0)
	{
		hexManager->highlightsOff();
		return;
	}

	int targetHex = hexManager->findClosestTarget(characterHex, targetLocations, m_selectedCharacter->m_range);
	
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Target hexInt: %i"),
		targetHex));

	if (targetHex >= 0) 
	{
		m_selectedCharacter->AIAttack = hexManager->calculateMovement(characterMovement, targetHex, characterHex, m_selectedCharacter->m_movementLeft, m_selectedCharacter->m_range);
		m_selectedCharacter->AITarget = targetHex;
	}
	
	else if (targetHex < 0)
	{
		targetHex = hexManager->findClosestRangeTarget(characterHex, targetLocations, m_selectedCharacter->m_range);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Target hexInt: %i"),
			targetHex));

		hexManager->calculateMoveTowards(characterMovement, targetHex, characterHex, m_selectedCharacter->m_movementLeft, m_selectedCharacter->m_range);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Number of moves(pm): %i"),
		characterMovement.Num()));

	setIdle(m_selectedCharacter);
}

void APlayerManager::moveEnemy(int movement) 
{
	characterMovement.RemoveAt(0);
	FVector destination = hexManager->HexGridArray[movement]->getLocation() + FVector(0, 0, 7);
	m_selectedCharacter->moveToHex(destination);
	hexManager->highlightsOff();
	hexManager->HexGridArray[m_selectedCharacter->getHexLocation()]->setOccupied(false);
	hexManager->HexGridArray[movement]->setOccupied(true);
	m_selectedCharacter->setHexLocation(movement);
}


void APlayerManager::movePlayerCharacter(int destIndex)
{
	if (m_selectedCharacter && m_selectedCharacter->m_movementLeft>0)
	{
		
		if (!hexManager->HexGridArray[destIndex]->getOccupied() && m_selectedCharacter->isIdle())
		{
			hexManager->HexGridArray[m_selectedCharacter->getHexLocation()]->setOccupied(false);
			hexManager->HexGridArray[destIndex]->setOccupied(true);
			hexManager->highlightsOff();
			FVector destination = hexManager->HexGridArray[destIndex]->getLocation() + FVector(0, 0, 7);
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

void APlayerManager::characterClicked(APlayerCharacter* character)
{
	/*
	if (character->m_currentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overkill!"));
		return;
	}
	*/	

	if (m_selectedCharacter->getAttacking())
	{

		if (validateAttack(character->getHexLocation()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Attack!"));
			m_selectedCharacter->rotateTo(character->getLocation());
			if (character->updateHealth(true, m_selectedCharacter->getAttack()))
			{
				removeCharacter(*character);
			}
			if (--m_selectedCharacter->m_numberOfAttacksLeft <= 0)
			{
				setIdle(m_selectedCharacter);				
			}
		}
	}
}

void APlayerManager::whenHexClicked(AHexTile* hex)
{
	if (!m_selectedCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("No selected character"));
	}

	else if (validateMovement(hex->m_index))
	{
		if (!hex->m_occupied)
		{
			hexManager->calculateMovement(characterMovement, hex->m_index, m_selectedCharacter->m_hexLocationIndex, 
											m_selectedCharacter->m_movementLeft, 0);
			setIdle(m_selectedCharacter);
			//movePlayerCharacter(hex->m_index);
		}
		else if (hex->m_occupied)
		{
			occupiedHexClicked(hex->m_index);
		}
	}
}

void APlayerManager::removeCharacter(APlayerCharacter& character)
{
	character.m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character.m_TPose, true);
	character.m_playerCharacterMesh->SetSimulatePhysics(true);

	FRotator rotation{ UKismetMathLibrary::FindLookAtRotation(character.m_origin, m_selectedCharacter->getLocation()) };

	FVector impulse{ rotation.Vector() * -5000 };
	character.m_playerCharacterMesh->AddImpulse(impulse, "", true);
	hexManager->HexGridArray[character.m_hexLocationIndex]->setAttackHighightVisible(false);
	hexManager->HexGridArray[character.m_hexLocationIndex]->setOccupied(false);

	--numberOfCharacters;
	CharacterArray.RemoveAt(character.getIndex());
	setIndexes();

	if (character.m_type == CharacterType::ALLY)
	{
		PlayerDeath.Broadcast(character.m_ID);
	}

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
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Players win!"));
	}
}

void APlayerManager::sortByInitiative()
{
	CharacterArray.Sort();
	setIndexes();
	setSelectedCharacter(CharacterArray[turnIndex]);
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
			setSelectedCharacter(CharacterArray[++turnIndex]);
		}
		else
		{
			turnIndex = 0;
			setSelectedCharacter(CharacterArray[turnIndex]);
		}
	}
}

bool APlayerManager::setAttacking(bool ally)
{
	if (ally)
	{
		if (m_selectedCharacter->m_type == CharacterType::ENEMY)
		{
			return false;
		}
	}
	if (m_selectedCharacter)
	{
		if (m_selectedCharacter->rdyToAttack())
		{
			m_selectedCharacter->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedCharacter->m_attackAnim, true);
			m_selectedCharacter->m_state = m_selectedCharacter->ATTACKING;
			hexManager->highlightsOff();
			hexManager->highlightAttackTiles(m_selectedCharacter->getHexLocation(),m_selectedCharacter->m_range);
			return true;
		}
		return false;	
	}
	return false;
}

void APlayerManager::selectedIdle(bool ally)
{
	if (ally)
	{
		m_selectedCharacter->startIdling();
	}
}

void APlayerManager::setIdle(APlayerCharacter* character)
{
	
	if (characterMovement.Num() > 0)
	{
		moveEnemy(characterMovement[0]);
		return;
	}

	if (character->m_type == CharacterType::ENEMY)
	{
		character->m_state = character->IDLE;

		if (character->AIAttack == true)
		{			
			if (setAttacking())
			{
				for (APlayerCharacter* possibleTarget : CharacterArray)
				{
					if (possibleTarget->getHexLocation() == character->AITarget)
					{
						for (int i{ 0 }; i < character->m_numberOfAttacks; ++i);
						characterClicked(possibleTarget);
						break;
					}
				}
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can't set attacking!"));
			}
		}
		else if (character->AIAttack == false)
		{
			m_selectedCharacter->m_movementLeft = 0;
		}		
	}	


	hexManager->highlightsOff();
	character->m_state = character->IDLE;

	if (character->m_type == CharacterType::ALLY && character->m_movementLeft > 0)
	{
		character->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character->m_selectedAnim, true);
		hexManager->highlightTiles(character->getHexLocation(), character->m_movementLeft);
	}
	else
	{
		character->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character->m_idleAnim, true);
	}
}

bool APlayerManager::validateMovement(int hexIndex)
{
	if (hexManager->HexGridArray[hexIndex]->inRange && m_selectedCharacter->m_type == CharacterType::ALLY)
	{
		return true;
	}
	else
		return false;
}

bool APlayerManager::validateAttack(int hexIndex)
{
	if (hexManager->checkIfAdjacent(hexManager->HexGridArray[hexIndex], hexManager->HexGridArray[m_selectedCharacter->getHexLocation()], m_selectedCharacter->m_range))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Valid AF!"));
		return true;
	}
	else
		return false;
}
