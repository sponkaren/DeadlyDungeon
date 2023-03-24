// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuManager.h"
#include "HexTile.h"
#include "StatGenerator.h"
#include "DeadlyInstance.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMenuManager::AMenuManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Rotation = GetActorRotation() + FRotator(0, 180, 0);

}

// Called when the game starts or when spawned
void AMenuManager::BeginPlay()
{
	Super::BeginPlay();
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTile::StaticClass(), foundHex);
	//setHexDelegates();	
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

void AMenuManager::spawnAlivePlayers(TArray<FPlayerStruct>& players)
{
	int i{ 0 };
	for (FPlayerStruct stats : players)
	{
		if(i<foundHex.Num())
		{
			hexTile = Cast<AHexTile>(foundHex[i]);
			hexTile->setOccupied(true);
			FVector Location = foundHex[i]->GetActorLocation() + FVector(0, 0, 7);			
			FTransform SpawnTransform(Rotation, Location);

			APlayerCharacter* player = GetWorld()->SpawnActorDeferred<APlayerCharacter>(playerCharacter, SpawnTransform);
			player->Init(stats);
			player->FinishSpawning(SpawnTransform);
			player->createRenderTarget();
			player->CharSelect.AddDynamic(this, &AMenuManager::populateInfo);
		}
		++i;
	}

	//FVector Location = foundHex[hex]->GetActorLocation() + FVector(0, 0, 7);
	//Cast<AHexTile>(foundHex[hex])->setOccupied(true);
	//APlayerCharacter* newPlayer = GetWorld()->SpawnActor<APlayerCharacter>(playerCharacter, Location, Rotation);
}


void AMenuManager::showLastDungeonUnlock(int unlock)
{
	for (int i{ 0 }; i < unlock; ++i)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Heyo!"));
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

		FTransform SpawnTransform(Rotation, Location);

		APlayerCharacter* newPlayer = GetWorld()->SpawnActorDeferred<APlayerCharacter>(playerCharacter, SpawnTransform);
		FPlayerStruct stats{ StatGenerator::generateStats(1,m_ID)};
		newPlayer->Init(stats);
		newPlayer->FinishSpawning(SpawnTransform);
		newPlayer->createRenderTarget();
		newPlayer->CharSelect.AddDynamic(this, &AMenuManager::populateInfo);
		populateInfo(newPlayer);
		CharacterCreated.Broadcast(newPlayer);
	}
}

void AMenuManager::getHex()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTile::StaticClass(), foundHex);
	setHexDelegates();
}

bool AMenuManager::characterExists()
{
	bool exists{ false };

	for (AActor* actor : foundHex)
	{
		hexTile = Cast<AHexTile>(actor);
		if (hexTile->getOccupied() == true)
			exists = true;
	}
	return exists;
}

void AMenuManager::populateInfo(APlayerCharacter* character)
{
	if (!unitInfoWidget)
	{
		return;
	}

	if (populator)
	{
		populator->infoPopup = false;
	}

	populator = character;
	character->infoPopup = true;

	unitInfoWidget->setText(character->getStats());

	unitInfoWidget->setImage(character->iconMaterial);
}