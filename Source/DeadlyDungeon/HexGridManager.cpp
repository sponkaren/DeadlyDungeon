// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"
#include "AxialPixelConversion.h"
#include "assert.h"
#include <cstdlib>

// Sets default values
AHexGridManager::AHexGridManager()
{

}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();

	numberOfTiles= m_gridWidth * m_gridHeight;
	HexGridArray.SetNumZeroed(numberOfTiles);
	
	FRotator Rotation = GetActorRotation();
	
	// always starting grid at position 0,0,0
	int r{ 0 };
	int q{ 0 };
	int indexX{ 0 };
	int indexY{ 0 };

	
	for (int tileNumber{ 0 }; tileNumber < numberOfTiles; tileNumber++)
	{
		TSubclassOf<AHexTile> tileToSpawn = m_waterHexTile;
		FVector Location = AxialPixelConversion::axialToPixel(r, q);
		
		AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>
			(tileToSpawn, Location, Rotation);
		
		//setting some tile data
		newTile->setAxial(r, q);
		newTile->setLocation(Location);
		newTile->setIndex(tileNumber);	

		HexGridArray[tileNumber] = newTile;
		
		newTile->HexTileClicked.AddDynamic(this, &AHexGridManager::whenHexClicked);

		int qTopValue{ m_gridHeight - (r + 1) / 2 - (r + 1) % 2 };
		if (q < qTopValue)
		{
			++q;
			indexY++;
		}
		else
		{
			r++;
			q = qTopValue - m_gridHeight + r % 2;

			indexX++;
			indexY = 0;

		}
	}

}

void AHexGridManager::whenHexClicked(AHexTile* hex)
{
	if (hex->m_tileType == EHexTileType::MENU)
	{
		if (hex->getOccupied())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Slot Occupied"));
			return;
		}
		else
		{
			//APlayerInventory::addPlayerCharacter(getLocation());
			hex->setOccupied(true);
			return;
		}
	}

	thisHexClicked.Broadcast(hex);
}

bool AHexGridManager::checkIfAdjacent(AHexTile* h1, AHexTile* h2, int range)
{
	int diffR = std::abs(h1->getAxialR() - h2->getAxialR());
	int diffQ = std::abs(h1->getAxialQ() - h2->getAxialQ());
	int diffS = std::abs(h1->getAxialS() - h2->getAxialS());

	if (diffR == 0 && diffQ == 0 && diffS == 0)
	{
		return false;
	}

	return((diffR <= range) && (diffQ <= range) && (diffS <= range));
}

void AHexGridManager::highlightTiles(int hexIndex) 
{
	for (AHexTile* hex : HexGridArray)
	{
		if (checkIfAdjacent(hex, HexGridArray[hexIndex])
			&& hex != HexGridArray[hexIndex]
			&& !hex->getOccupied())
		{
			hex->setHighightVisible(true);
		}
	}
}

void AHexGridManager::highlightAttackTiles(int hexIndex)
{
	
	for (AHexTile* hex : HexGridArray)
	{
		if (checkIfAdjacent(hex, HexGridArray[hexIndex])
			&& hex != HexGridArray[hexIndex]
			&& hex->getOccupied())
		{
			hex->setAttackHighightVisible(true);
		}
	}
}

void AHexGridManager::highlightsOff()
{
	for (AHexTile* hex : HexGridArray)
	{
		hex->setHighightVisible(false);
		hex->setAttackHighightVisible(false);
	}
}

AHexTile* AHexGridManager::findHexByAxial(int Q, int R)
{
	for (AHexTile* hex : HexGridArray)
	{
		if (hex->m_axialQ == Q && hex->m_axialR == R)
		{
			return hex;
		}
	}	
	return nullptr;
}

int AHexGridManager::getNextPlayerSpawn()
{
	for(int i{(m_gridWidth/4)*m_gridHeight};i<HexGridArray.Num();i+=m_gridHeight)
	{
		if (!HexGridArray[i]->getOccupied())
		{
			HexGridArray[i]->setOccupied(true);
			return i;
		}
	}
	return 999;
}

int AHexGridManager::getNextEnemySpawn()
{
	for (int i{ ((m_gridWidth / 4) * m_gridHeight)-1}; i < HexGridArray.Num(); i += m_gridHeight)
	{
		if (!HexGridArray[i]->getOccupied())
		{
			HexGridArray[i]->setOccupied(true);
			return i;
		}
	}
	for (int i{ m_gridHeight - 2 }; i < HexGridArray.Num(); i += m_gridHeight)
	{
		if (!HexGridArray[i]->getOccupied())
		{
			HexGridArray[i]->setOccupied(true);
			return i;
		}
	}
	return 999;
}

int AHexGridManager::findClosestTarget(int hexIndex, const TArray<int>& targets, int range)
{
	int targetIndex{-1};
	int lowestValidPrio{999};

	for (int possibleTarget : targets)
	{
		setPriorities(possibleTarget);

		int prio = HexGridArray[hexIndex]->movePrio;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Prio: %i"),
		//	prio));

		//target is self
		if (prio == 0)
		{
			return HexGridArray[hexIndex]->movePrio;
		}

		else if (prio > 0 && prio < lowestValidPrio)
		{
			lowestValidPrio = prio;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Set lowest prio: %i"),
			//	lowestValidPrio));
			targetIndex = HexGridArray[possibleTarget]->m_index;
		}
	}
	return targetIndex;
}

int AHexGridManager::findClosestRangeTarget(int hexIndex, const TArray<int>& targets, int range)
{
	
	int targetIndex{ -1 };
	int closestRange{ 999 };

	for (int possibleTarget : targets)
	{
		int range = HexGridArray[hexIndex]->getDistance(*HexGridArray[possibleTarget]);
		if (range < closestRange)
		{
			closestRange = range;
			targetIndex = HexGridArray[possibleTarget]->m_index;
		}
	}

	return targetIndex;
}

bool AHexGridManager::calculateMovement(TArray<int>& movementArray, int targetHex, int hexIndex, int movementLeft, int range)
{
	setPriorities(targetHex, range);

	int locPrio = HexGridArray[hexIndex]->movePrio;

	//Already in prime position, no need for calculations
	if (locPrio == 1)
	{
		clearPriorities();
		return true;
	}

	movementCalc(movementArray, hexIndex, movementLeft, locPrio);
	clearPriorities();
	
	if (locPrio == 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Returning true!"));
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Returning false!"));
		return false;
	}
}

void AHexGridManager::calculateMoveTowards(TArray<int>& movementArray, int targetHex, int hexIndex, int movementLeft, int range)
{
	setPriorities(targetHex, range, true);
	int locPrio = HexGridArray[hexIndex]->movePrio;

	movementCalc(movementArray, hexIndex, movementLeft, locPrio);
}

void AHexGridManager::movementCalc(TArray<int>& movementArray, int hexIndex, int movementLeft, int& locPrio)
{
	int q{ HexGridArray[hexIndex]->m_axialQ };
	int r{ HexGridArray[hexIndex]->m_axialR };

	for (int i{ 0 }; i < movementLeft; ++i)
	{
		bool nextFound{ false };
		int nextIndex{};

		for (AHexTile* hex : HexGridArray)
		{
			if (checkIfAdjacent(findHexByAxial(q, r), hex) && hex->getOccupied() == false)
			{
				bool traveled = false;

				for (int destination : movementArray)
				{
					if (hex->m_index == destination)
					{
						traveled = true;
					}
				}
				if (!traveled)
				{
					int destPrio = hex->movePrio;
					if (destPrio > 0 && destPrio < locPrio)
					{
						locPrio = destPrio;
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Set new locPrio: %i"),
							//locPrio));
						nextIndex = hex->m_index;
						nextFound = true;
					}
				}
			}
		}
		if (nextFound)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Adding index: %i"),
			//	nextIndex));
			movementArray.Emplace(nextIndex);
			q = HexGridArray[nextIndex]->m_axialQ;
			r = HexGridArray[nextIndex]->m_axialR;
		}
	}
}

int AHexGridManager::getAttractiveness(AHexTile* start, AHexTile* next, AHexTile* end)
{
	int diffQ = std::abs(start->getAxialQ() - end->getAxialQ());
	int diffR = std::abs(start->getAxialR() - end->getAxialR());	
	int diffS = std::abs(start->getAxialS() - end->getAxialS());

	int ndiffQ = std::abs(next->getAxialQ() - end->getAxialQ());
	int ndiffR = std::abs(next->getAxialR() - end->getAxialR());
	int ndiffS = std::abs(next->getAxialS() - end->getAxialS());

	return diffQ + diffR + diffS - ndiffQ - ndiffR - ndiffS;

}

void AHexGridManager::setPriorities(int targetHex, int range, bool rangeCheck)
{
	clearPriorities();

	HexGridArray[targetHex]->movePrio = 0;
	
	int checked{1};

	for (int p{ 0 }; p < numberOfTiles; ++p)
	{
		for (AHexTile* hex : HexGridArray)
		{
			if (hex->movePrio == p && (hex->moveBlock == false || rangeCheck == true))
			{
				for (AHexTile* hex2 : HexGridArray)
				{
					if (checkIfAdjacent(hex, hex2, range))
					{
						++checked;
						if (hex2->movePrio == -1)
						{
							if (hex2->m_occupied)
							{
								hex2->moveBlock = true;
							}
							hex2->movePrio = p + 1;
						}
					}
				}

			}
		}
		if (checked == numberOfTiles)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("All checked!"));
			break;
		}
	}
}

void AHexGridManager::clearPriorities()
{
	for (AHexTile* hex : HexGridArray)
	{
		hex->moveBlock = false;
		hex->movePrio = -1;	
	}
}

int AHexGridManager::getTargetPriority(int hexIndex, int targetIndex)
{
	setPriorities(targetIndex);
	int tp = HexGridArray[hexIndex]->movePrio;
	clearPriorities();
	return tp;
}