// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"

// Sets default values
AHexGridManager::AHexGridManager()
{
}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();
	
	HexGrid2DArray.SetNumZeroed(m_gridWidth);
	for (int32 i{ 0 }; i < HexGrid2DArray.Num(); ++i)
	{
		HexGrid2DArray[i].SetNumZeroed(m_gridHeight);
	}

	const int numberOfTiles{ m_gridWidth * m_gridHeight };
	
	FRotator Rotation = GetActorRotation();
	// always starting grid at position 0,0,0
	int r{ 0 };
	int q{ 0 };
	int indexX{ 0 };
	int indexY{ 0 };

	
	for (int tileNumber{ 1 }; tileNumber <= numberOfTiles; tileNumber++)
	{
		TSubclassOf<AHexTile> tileToSpawn = m_grassHexTile;

		AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>
			(tileToSpawn, AxialPixelConversion::axialToPixel(r, q), Rotation);
		newTile->setAxial(r, q);
		newTile->setIndex(indexX, indexY);
		HexGrid2DArray[indexX][indexY] = newTile;

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

