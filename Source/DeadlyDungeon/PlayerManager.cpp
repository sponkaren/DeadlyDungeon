// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"
#include <algorithm>
#include <array>
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LatentActionManager.h"
#include "Components/Image.h"
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

void APlayerManager::turnActionWidgetSetup(UTurnActionWidget* widget)
{
	turnActionWidget = widget;	
	turnActionWidget->NextTurn.AddDynamic(this, &APlayerManager::setNextTurn);
	turnActionWidget->Execute.AddDynamic(this, &APlayerManager::executeClicked);
}

void APlayerManager::turnOrderWidgetSetup(UTurnOrderWidget* widget)
{
	turnOrderWidget = widget;
	turnOrderWidget->HoverChange.AddDynamic(this, &APlayerManager::hoverChange);
	turnOrderWidget->IconClicked.AddDynamic(this, &APlayerManager::populateInfo);
}

void APlayerManager::unitInfoWidgetSetup(UUnitInfoWidget* widget)
{
	unitInfoWidget = widget;
}

void APlayerManager::populateInfo(APlayerCharacter* character)
{
	if (!unitInfoWidget)
	{
		return;
	}

	if (populator)
	{
		populator->infoPopup = false;
	}

	populator = character;
	character->infoPopup = true;

	unitInfoWidget->setText(character->getStats());

	unitInfoWidget->setImage(character->iconMaterial);

}

void APlayerManager::setTurnOrderIcons()
{
	for (APlayerCharacter* player : CharacterArray)
	{
		addIconTurnOrder(player->iconMaterial, player);
	}

	if (CharacterArray.Num() > 0)
	{
		populateInfo(CharacterArray[0]);		
	}
}

void APlayerManager::addIconTurnOrder(UMaterialInterface* characterIcon, APlayerCharacter* character)
{	
	turnOrderWidget->createUnitIcon(characterIcon, character);
}

void APlayerManager::handlePlayersToSpawn(TArray<FPlayerStruct>& players)
{
	
	for (FPlayerStruct stats : players)
	{
		spawnPlayer(stats, hexManager->getNextPlayerSpawn(players.Num()), false);
	}
	
}

void APlayerManager::spawnEnemies(int difficulty, int numberOfEnemies)
{
	for (int i{ 0 }; i < numberOfEnemies; ++i)
	{
		FPlayerStruct stats{ StatGenerator::generateStats(difficulty) };
		spawnPlayer(stats, hexManager->getNextEnemySpawn(numberOfEnemies), true);
	}
}

void APlayerManager::spawnPlayer(FPlayerStruct& stats, int hexIndex, bool enemy)
{	

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
	newPlayer->createRenderTarget();

	newPlayer->CharClicked.AddDynamic(this, &APlayerManager::characterClicked);
	newPlayer->CharIdle.AddDynamic(this, &APlayerManager::setIdle);
	newPlayer->CharShot.AddDynamic(this, &APlayerManager::characterShot);
	newPlayer->CharSelect.AddDynamic(this, &APlayerManager::populateInfo);

	//addIconTurnOrder(newPlayer->iconMaterial);
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
		lastClicked->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character->m_idleAnim, true);
		lastClicked->setArrowOn(false);
		hexManager->highlightsOff();
	}

	lastClicked = character;
	character->setArrowOn(true);
	character->resetMoveAtk();
	characterMovement.Empty();
	character->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character->m_selectedAnim, true);
	APlayerManager::storeSelectedCharacter(character);
	//hexManager->highlightTiles(character->getHexLocation(),m_selectedCharacter->m_movementLeft);	
	
	if (character->m_type == CharacterType::ENEMY)
	{
		startAI();
	}
	else
	{
		setIdle(m_selectedCharacter);
	}
}

void APlayerManager::startAI()
{
	int characterHex{m_selectedCharacter->getHexLocation() };
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
	
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Target hexInt: %i"),
	//	targetHex));

	if (targetHex >= 0) 
	{
		m_selectedCharacter->AIAttack = hexManager->calculateMovement(characterMovement, targetHex, characterHex, m_selectedCharacter->m_movementLeft, m_selectedCharacter->m_range);
		m_selectedCharacter->AITarget = targetHex;
	}
	
	else if (targetHex < 0)
	{
		targetHex = hexManager->findClosestRangeTarget(characterHex, targetLocations, m_selectedCharacter->m_range);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Target hexInt: %i"),
		//	targetHex));

		hexManager->calculateMoveTowards(characterMovement, targetHex, characterHex, m_selectedCharacter->m_movementLeft, m_selectedCharacter->m_range);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Number of moves(pm): %i"),
	//	characterMovement.Num()));

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
	if (character == m_selectedCharacter)
	{
		resetCommands();
		return;
	}
	if (m_selectedCharacter->AIAttack == true)
	{
		if (character->getHexLocation() == m_selectedCharacter->AITarget)
		{	
			hexManager->HexGridArray[m_selectedCharacter->AITarget]->setAttackSelectHighightVisible(false);
			m_selectedCharacter->AIAttack = false;
			return;
		}
		else
			resetCommands();
	}
	if (hexManager->HexGridArray[character->m_hexLocationIndex]->atkHighlight == true)
	{	
		int movements{ characterMovement.Num() };
		bool chainable{ false };

		if (movements > 0)
		{
			m_selectedCharacter->AIAttack = hexManager->calculateMovement(characterMovement, character->getHexLocation(), characterMovement[movements - 1],
				m_selectedCharacter->m_movementLeft - movements, m_selectedCharacter->m_range, true);
			if (m_selectedCharacter->AIAttack)
			{
				chainable = true;
			}			
		}
		if (!chainable)
		{
			resetCommands();
			m_selectedCharacter->AIAttack = hexManager->calculateMovement(characterMovement, character->getHexLocation(), m_selectedCharacter->getHexLocation(), m_selectedCharacter->m_movementLeft, m_selectedCharacter->m_range, true);
		}
		
		m_selectedCharacter->AITarget = character->getHexLocation();
		hexManager->HexGridArray[m_selectedCharacter->AITarget]->setAttackSelectHighightVisible(true);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Out of range!"));
}

void APlayerManager::characterAttacked(APlayerCharacter* character)
{
	if (validateAttack(character->getHexLocation()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Attack!"));
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

void APlayerManager::whenHexClicked(AHexTile* hex)
{
	if (!m_selectedCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("No selected character"));
	}
	else if (m_selectedCharacter->getHexLocation() == hex->m_index)
	{
		resetCommands();
		return;
	}
	else if (validateMovement(hex->m_index))
	{
		if (!hex->m_occupied)
		{
			int movements{characterMovement.Num()};
			bool chainable{ false };

			if (movements > 0)
			{
				if (hexManager->calculateMovement(characterMovement, hex->m_index, characterMovement[movements - 1],
					m_selectedCharacter->m_movementLeft-movements, 0))				
					chainable = true;							
			}

			if (!chainable)
			{
				resetCommands();
				hexManager->calculateMovement(characterMovement, hex->m_index, m_selectedCharacter->m_hexLocationIndex, m_selectedCharacter->m_movementLeft, 0);				
			}

			hexManager->highlightMovement(characterMovement);
		}
		else if (hex->m_occupied)
		{
			occupiedHexClicked(hex->m_index);
		}
	}
	
	else if (hex->atkHighlight == true)
	{
		for (APlayerCharacter* possibleTarget : CharacterArray)
		{
			if (possibleTarget->getHexLocation() == hex->m_index)
				characterClicked(possibleTarget);
		}
	}
}

void APlayerManager::removeCharacter(APlayerCharacter& character, bool shot)
{
	character.m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character.m_TPose, true);
	character.m_playerCharacterMesh->SetSimulatePhysics(true);

	
	FRotator rotation{ UKismetMathLibrary::FindLookAtRotation(character.m_origin, m_selectedCharacter->getLocation()) };
	character.shootCorpse(rotation);
	

	//FVector impulse{ rotation.Vector() * -5000 };
	//character.m_playerCharacterMesh->AddImpulse(impulse, "", true);
	hexManager->HexGridArray[character.m_hexLocationIndex]->setAttackHighightVisible(false);
	hexManager->HexGridArray[character.m_hexLocationIndex]->setOccupied(false);

	--numberOfCharacters;
	int deadIndex{ character.getIndex() };
	turnOrderWidget->deleteIcon(deadIndex);
	CharacterArray.RemoveAt(deadIndex);
	
	if (turnIndex >= deadIndex)
	{
		--turnIndex;
	}

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
		GameOver.Broadcast(false);
	}
	else
	{
		GameOver.Broadcast(true);
	}
}

void APlayerManager::sortByInitiative()
{
	CharacterArray.Sort();
	setIndexes();
	setTurnOrderIcons();
	setSelectedCharacter(CharacterArray[turnIndex]);
	turnOrderWidget->setActiveTurn(turnIndex);
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
			turnOrderWidget->setActiveTurn(++turnIndex);
			setSelectedCharacter(CharacterArray[turnIndex]);
		}
		else
		{
			turnIndex = 0;
			turnOrderWidget->setActiveTurn(turnIndex);
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
		hexManager->highlightMovement(characterMovement, false);
		moveEnemy(characterMovement[0]);
		return;
	}

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
					characterAttacked(possibleTarget);
					break;
				}
			}
		}
	}
	else if (character->m_type == CharacterType::ENEMY && character->AIAttack == false)
	{
		m_selectedCharacter->m_movementLeft = 0;
		character->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character->m_idleAnim, true);	
	}		
	
	hexManager->highlightsOff();
	character->m_state = character->IDLE;


	if (character->m_type == CharacterType::ALLY)
	{
		if (character->m_movementLeft > 0)
		{
			hexManager->highlightTiles(character->getHexLocation(),m_selectedCharacter->m_movementLeft);	
		}
		else
		{
			character->m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(character->m_idleAnim, true);
		}

		TArray<int> targetLocations;

		for (APlayerCharacter* possibleTarget : CharacterArray)
		{
			targetLocations.Emplace(possibleTarget->getHexLocation());
		}
		if (character->m_numberOfAttacksLeft > 0)
		{
			hexManager->findClosestTarget(m_selectedCharacter->getHexLocation(), targetLocations, m_selectedCharacter->m_range, true, m_selectedCharacter->m_movementLeft);
		}
	}
}

void APlayerManager::executeClicked(bool active)
{
	if (active)
	{
		setIdle(m_selectedCharacter);
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
		return true;
	}
	else
		return false;
}

void APlayerManager::characterShot(APlayerCharacter* character)
{
	if (character->updateHealth(true, 50))
	{
		removeCharacter(*character);
	}
}

void APlayerManager::resetCommands()
{
	if (characterMovement.Num() > 0)
	{
		hexManager->highlightMovement(characterMovement, false);
		characterMovement.Empty();
	}
	hexManager->HexGridArray[m_selectedCharacter->AITarget]->setAttackSelectHighightVisible(false);
	m_selectedCharacter->AIAttack = false;	
}

void APlayerManager::hoverChange(int index, bool on)
{
	if (index != m_selectedCharacter->m_index)
	{
		CharacterArray[index]->setArrowOn(on);
	}

	CharacterArray[index]->beingHovered = on;
}