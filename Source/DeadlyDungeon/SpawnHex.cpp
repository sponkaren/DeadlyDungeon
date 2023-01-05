// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnHex.h"

// Sets default values
ASpawnHex::ASpawnHex()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnHex::BeginPlay()
{
	Super::BeginPlay();
	
	const int gridWidth{ 8 };
	const int gridHeight{ 8 };
	
	int r{ 0 };
	int q{ 0 };

	const int numberOfTiles{ gridWidth * gridHeight };
	
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	for (int tileNumber{ 1 }; tileNumber <= numberOfTiles; tileNumber++)
	{

		// always starting grid at position 0,0,0

		GetWorld()->SpawnActor<AActor>(HexToSpawn, AxialPixelConversion::axialToPixel(r, q) , Rotation);


		int qTopValue{ gridHeight - (r + 1) / 2 - (r + 1) % 2 };

		if (q < qTopValue)
		{
			++q;
		}

		else
		{
			r++;
			q = qTopValue - gridHeight + r % 2;
		}
	}
}



