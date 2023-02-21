// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuManager.h"
#include "HexTile.h"
#include "DeadlyInstance.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMenuManager::AMenuManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMenuManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTile::StaticClass(), foundHex);
	setHexDelegates();	
}

// Called every frame
void AMenuManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMenuManager::setHexDelegates()
{
	for (AActor* actor : foundHex)
	{
		hexTile = Cast<AHexTile>(actor);
		hexTile->HexTileClicked.AddDynamic(this, &AMenuManager::whenHexClicked);
	}
}


void AMenuManager::whenHexClicked(AHexTile* hex)
{
	if (hex->m_occupied)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Occupied!"));
	else
	{
		hex->setOccupied(true);

		FVector Location = hex->getLocation() + FVector(0,0,7);
		FRotator Rotation = GetActorRotation() +FRotator(0,180,0);

		APlayerCharacter* newPlayer = GetWorld()->SpawnActor<APlayerCharacter> (playerCharacter, Location, Rotation);	

		CharacterCreated.Broadcast(newPlayer);
	}
}
