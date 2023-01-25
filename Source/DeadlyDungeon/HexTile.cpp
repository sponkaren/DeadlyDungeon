// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTile.h"

// Sets default values
AHexTile::AHexTile()
{
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_tileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	m_tileMesh->SetupAttachment(m_rootComponent);
	occupied = false;
}

void AHexTile::setAxial(int r, int q)
{
	m_axialR = r;
	m_axialQ = q;
	m_axialS = -r - q;
	
	this->SetActorLabel(FString::Printf(TEXT("Tile: r:%d, q:%d"),r,q));
}

void AHexTile::setIndex(int i)
{
	m_index = i;
}

