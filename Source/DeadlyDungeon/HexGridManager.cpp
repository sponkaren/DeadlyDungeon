// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"
#include "AxialPixelConversion.h"
#include <cstdlib>

// Sets default values
AHexGridManager::AHexGridManager()
{

}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();

	const int numberOfTiles{ m_gridWidth * m_gridHeight };
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

bool AHexGridManager::checkIfAdjacent(AHexTile* h1, AHexTile* h2)
{
	int diffR = std::abs(h1->getAxialR() - h2->getAxialR());
	int diffQ = std::abs(h1->getAxialQ() - h2->getAxialQ());
	int diffS = std::abs(h1->getAxialS() - h2->getAxialS());

	if (diffR == 0 && diffQ == 0 && diffS == 0)
	{
		return false;
	}

	return((diffR <= 1) && (diffQ <= 1) && (diffS <= 1));
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