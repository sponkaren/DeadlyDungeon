// Fill out your copyright notice in the Description page of Project Settings.

#include "HexTile.h"
#include "AxialPixelConversion.h"

// Sets default values
AHexTile::AHexTile()
{
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	m_tileMesh->SetupAttachment(m_rootComponent);

	m_location = FVector(0, 0, 0);

	occupied = false;
}

void AHexTile::setAxial(int r, int q)
{
	m_axialR = r;
	m_axialQ = q;
	m_axialS = -r - q;
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
	return m_location;
}


