#include "HexTile.h"
#include <cmath>
#include <cstdlib>


AHexTile::AHexTile()
{
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	m_tileHighlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileHighlight"));
	m_tileAttackHighlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileAttackHighlight"));
	m_tileSelectHighlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileSelectHighlight"));
	m_tileAttackSelectHighlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileAttackSelectHighlight"));
	m_tileMesh->SetupAttachment(m_rootComponent);
	m_tileHighlight->SetupAttachment(m_rootComponent);
	m_tileAttackHighlight->SetupAttachment(m_rootComponent);
	m_tileSelectHighlight->SetupAttachment(m_rootComponent);
	m_tileAttackSelectHighlight->SetupAttachment(m_rootComponent);
	m_location = FVector(0, 0, 0);
	m_occupied = false;
}

void AHexTile::setAxial(int r, int q)
{
	m_axialR = r;
	m_axialQ = q;
	m_axialS = -r - q;
}

int AHexTile::getAxialR()
{
	return m_axialR;
}

int AHexTile::getAxialQ()
{
	return m_axialQ;
}

int AHexTile::getAxialS()
{
	return m_axialS;
}

void AHexTile::setIndex(int i)
{
	m_index = i;

	this->SetActorLabel(FString::Printf(TEXT("Tile: Index:%d"), m_index));
}
 
void AHexTile::setLocation(FVector vector)
{
	m_location = vector;
}

FVector AHexTile::getLocation()
{
	return GetActorLocation();
}

void AHexTile::setOccupied(bool occupied)
{
	m_occupied = occupied;
}

bool AHexTile::getOccupied()
{
	return m_occupied;
}

void AHexTile::hexClicked()
{
	HexTileClicked.Broadcast(this);
}


void AHexTile::setHighightVisible(bool on)
{
	m_tileHighlight->SetVisibility(on, true);
	inRange = on;
}

void AHexTile::setAttackHighightVisible(bool on)
{
	m_tileAttackHighlight->SetVisibility(on, true);
	atkHighlight = on;
}

void AHexTile::setMoveSelectVisible(bool on)
{
	m_tileSelectHighlight->SetVisibility(on, true);
}

void AHexTile::setAttackSelectHighightVisible(bool on)
{
	m_tileAttackSelectHighlight->SetVisibility(on, true);	
}

int AHexTile::getDistance(const AHexTile& refHex)
{
	for (int i{ 1 };i<100; i++)
	{
		if (std::abs(m_axialQ - refHex.m_axialQ) <= i)
		{
			if (std::abs(m_axialR - refHex.m_axialR) <= i)
			{
				if (std::abs(m_axialS - refHex.m_axialS) <= i)
				{
					return i;
				}
			}
		}
	}
	return 100;
}

void AHexTile::setMovePrio(int prio, bool debug)
{
	movePrio = prio;
	
	if (debug)
	{
		displayMovePrio();
	}
}

void AHexTile::displayMovePrio()
{
	if (movePrio != -1)
	{
		movePrioOn = true;
	}
}

AHexTile& AHexTile::getHex()
{
	return *this;
}