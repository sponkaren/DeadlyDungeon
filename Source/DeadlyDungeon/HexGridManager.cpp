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

void AHexGridManager::setAdjacentHex()
{
	for (AHexTile* hex : HexGridArray)
	{
		int count = 0;
		for (AHexTile* hex2 : HexGridArray)
		{
			if (checkIfAdjacent(hex, hex2))
			{
				hex->adjacentHex.Emplace(hex2);
				if (++count == 6)
					break;

			}			
		}
	}
}

void AHexGridManager::highlightTiles(int hexIndex, int movement) 
{	
	for (AHexTile* hex : HexGridArray[hexIndex]->adjacentHex)
	{
		if (!hex->getOccupied())
		{
			hex->setHighightVisible(true);
			if (movement>1)
			{
				highlightTiles(hex->m_index, movement-1);
			}			
		}
	}
}

void AHexGridManager::highlightAttackTiles(int hexIndex, int range)
{
	
	for (AHexTile* hex : HexGridArray)
	{
		if (checkIfAdjacent(hex, HexGridArray[hexIndex], range)
			&& hex != HexGridArray[hexIndex]
			&& hex->getOccupied())
		{
			hex->setAttackHighightVisible(true);
		}
	}
}

void AHexGridManager::highlightMovement(const TArray<int>& movementArray, bool on)
{
	for (int hexIndex : movementArray)
	{
		HexGridArray[hexIndex]->setMoveSelectVisible(on);
	}
}

void AHexGridManager::highlightsOff()
{
	for (AHexTile* hex : HexGridArray)
	{
		hex->setHighightVisible(false);
		hex->setAttackHighightVisible(false);
		hex->setMoveSelectVisible(false);
		hex->setAttackSelectHighightVisible(false);
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

int AHexGridManager::getNextPlayerSpawn(int players)
{
	for(int i{(m_gridWidth/players)*m_gridHeight};i<HexGridArray.Num();i+=m_gridHeight)
	{
		if (!HexGridArray[i]->getOccupied())
		{
			HexGridArray[i]->setOccupied(true);
			return i;
		}
	}
	for (AHexTile* hex : HexGridArray)
	{
		if (!hex->m_occupied)
		{
			hex->setOccupied(true);
			return hex->m_index;
		}
	}
	return 999;
}

int AHexGridManager::getNextEnemySpawn(int enemies)
{
	for (int r{ 1 }; r < 3; ++r)
	{
		for (int i{ ((m_gridWidth / enemies) * m_gridHeight) - r }; i < HexGridArray.Num(); i += m_gridHeight)
		{
			if (!HexGridArray[i]->getOccupied())
			{
				HexGridArray[i]->setOccupied(true);
				return i;
			}
		}
	}

	for (AHexTile* hex : HexGridArray)
	{
		if (!hex->m_occupied)
		{
			hex->setOccupied(true);
			return hex->m_index;
		}
	}
	return 999;
}


int AHexGridManager::findClosestTarget(int hexIndex, const TArray<int>& targets, int range, bool highlighting, int movementLeft)
{
	int targetIndex{-1};
	int lowestValidPrio{999};

	for (int possibleTarget : targets)
	{
		setPriorities(possibleTarget, hexIndex, range);

		int prio = HexGridArray[hexIndex]->movePrio;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Prio: %i"),
		//	prio));
		
		if (highlighting)
		{	
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Checking!"));

			if (prio <= movementLeft+range && prio != 0)
			{
				HexGridArray[possibleTarget]->setAttackHighightVisible(true);
			}
		}

		//target is self
		if (prio == 0 && highlighting == false)
		{
			return HexGridArray[hexIndex]->movePrio;
		}

		else if (prio > 0 && prio < lowestValidPrio && highlighting==false)
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

bool AHexGridManager::calculateMovement(TArray<int>& movementArray, int targetHex, int hexIndex, int movementLeft, int range, bool highlighting)
{
	setPriorities(targetHex, hexIndex, range);

	int locPrio = HexGridArray[hexIndex]->movePrio;
	
	//Already in prime position, no need for calculations
	if (locPrio == range)
	{
		clearPriorities();
		return true;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("locPrio: %i"),
	//	locPrio));

	movementCalc(movementArray, hexIndex, movementLeft, locPrio, range, highlighting);
	clearPriorities();
	
	if (locPrio <= range)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Returning true!"));
		return true;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Returning false!"));
		return false;
	}
}

void AHexGridManager::calculateMoveTowards(TArray<int>& movementArray, int targetHex, int hexIndex, int movementLeft, int range)
{
	setPriorities(targetHex, hexIndex, range, true);
	int locPrio = HexGridArray[hexIndex]->movePrio;

	movementCalc(movementArray, hexIndex, movementLeft, locPrio, range);
}

void AHexGridManager::movementCalc(TArray<int>& movementArray, int hexIndex, int movementLeft, int& locPrio, int range, bool highlighting)
{
	int q{ HexGridArray[hexIndex]->m_axialQ };
	int r{ HexGridArray[hexIndex]->m_axialR };

	for (int i{ 0 }; i < movementLeft; ++i)
	{
		bool nextFound{ false };
		int nextIndex{};

		for (AHexTile* hex : findHexByAxial(q, r)->adjacentHex)
		{
			if (hex->getOccupied() == false)
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
					if (destPrio >= 0 && destPrio < locPrio)
					{
						locPrio = destPrio;
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Set new locPrio: %i"),
						//	locPrio));
						nextIndex = hex->m_index;
						nextFound = true;
					}
				}
			}
		}
		/*
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
		*/

		if (nextFound)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Adding index: %i"),
				nextIndex));
			movementArray.Emplace(nextIndex);
			if (highlighting)
			{
				HexGridArray[nextIndex]->setMoveSelectVisible(true);
			}
			if (locPrio <= range)
				return;
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

void AHexGridManager::setPriorities(int targetHex, int locHex, int range, bool rangeCheck)
{		
	clearPriorities();
	hexIndexes.Empty();	
	
	int p{ 0 };	
	int checks{ 0 };

	HexGridArray[targetHex]->movePrio = p;	
	hexIndexes.Emplace(targetHex);
	HexGridArray[targetHex]->gStar = getGStar(HexGridArray[targetHex], HexGridArray[locHex]);
	int bestG{HexGridArray[targetHex]->gStar};

	for (int i{0};i<hexIndexes.Num(); ++i)
	{		
		int hexIndex = hexIndexes[i];

		if (!HexGridArray[hexIndex]->moveBlock || rangeCheck)
		{
			for (AHexTile* hex : HexGridArray[hexIndex]->adjacentHex)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Checks done: %i"),
				//	++checks));
				
				if (hex->movePrio == -1)
				{
					if (hex->m_occupied && HexGridArray[hexIndex]->movePrio+1>=range)
					{
						hex->moveBlock = true;
					}
					
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Setting new moveprio: %i"),
					//HexGridArray[hexIndex]->movePrio + 1));

					hex->movePrio = HexGridArray[hexIndex]->movePrio+1;
					
					
					if (hex->m_index == locHex)
					{
						return;
					}					
					
					hexIndexes.Emplace(hex->m_index);										
					hex->gStar = getGStar(hex, HexGridArray[locHex]);
				}
			}
		}	
		sortByGStar(i);
	}
		




	/*
	for (int p{ 0 };; ++p)
	{
		pathFound = false;

		for (int hexIndex : hexIndexes)
		{
			if (HexGridArray[hexIndex]->movePrio == p && (!HexGridArray[hexIndex]->moveBlock || rangeCheck))
			{
				for(AHexTile* hex : HexGridArray[hexIndex]->adjacentHex)
				{
						if (hex->movePrio == -1)
						{
							if (hex->m_occupied)
							{
								hex->moveBlock = true;
							}
							hex->movePrio = p + 1;
							if (hex->m_index == locHex)
							{
								break;
							}
							pathFound = true;
						}
				}			
			}			
		}
		if (!pathFound)
		{
			break;
		}
	}
	*/
}

int AHexGridManager::getGStar(AHexTile* ref, AHexTile* end)
{
	int diffQ = std::abs(ref->getAxialQ() - end->getAxialQ());
	int diffR = std::abs(ref->getAxialR() - end->getAxialR());
	int diffS = std::abs(ref->getAxialS() - end->getAxialS());

	return diffQ + diffR + diffS;
}

void AHexGridManager::clearPriorities()
{
	for (AHexTile* hex : HexGridArray)
	{
		hex->moveBlock = false;
		hex->movePrio = -1;	
		hex->gStar = 0;
	}
}

void AHexGridManager::sortByGStar(int startIndex)
{
	int iA{ startIndex+ 1 };
	int iB{};

	for (iA; iA <= hexIndexes.Num(); ++iA)
	{
		iB = iA + 1;
		for (iB; iB < hexIndexes.Num();++iB)
		{
			if (HexGridArray[hexIndexes[iA]]->gStar > HexGridArray[hexIndexes[iB]]->gStar)
			{				
				int temp = hexIndexes[iA];
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Pusing temp (A): %i"),
				//	temp));

				hexIndexes.Push(temp);
				hexIndexes.RemoveAt(iA);

			}
			else
			{
				int temp = hexIndexes[iB];
			    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Pusing temp (B): %i"),
				//	temp));
				hexIndexes.Push(temp);
				hexIndexes.RemoveAt(iB);
			}
		}		
	}
}

void AHexGridManager::setIndexArray()
{
	hexIndexes.Empty();
	for (AHexTile* hex : HexGridArray)
	{
		hexIndexes.Emplace(hex->m_index);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Number of items in hexIndexes: %i"),
	//hexIndexes.Num()));
}