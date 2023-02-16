#include "HexTile.h"


AHexTile::AHexTile()
{
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	m_tileHighlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileHighlight"));
	m_tileAttackHighlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileAttackHighlight"));
	m_tileMesh->SetupAttachment(m_rootComponent);
	m_tileHighlight->SetupAttachment(m_rootComponent);
	m_tileAttackHighlight->SetupAttachment(m_rootComponent);
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
}

void AHexTile::setAttackHighightVisible(bool on)
{
	m_tileAttackHighlight->SetVisibility(on, true);
}

AHexTile& AHexTile::getHex()
{
	return *this;
}