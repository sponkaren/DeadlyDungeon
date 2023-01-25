// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGridManager.h"
#include "PlayerCharacter.h"
#include "PlayerManager.h"

// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerManager::spawnOnClick()
{	
	numberOfPlayers++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Spawning time!"));


}

