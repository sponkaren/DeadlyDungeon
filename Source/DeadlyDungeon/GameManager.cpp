// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "PlayerManager.h"

int AGameManager::roundCount{};

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	roundCount = 1;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameManager::startGame()
{

}


void AGameManager::endGame(bool victory)
{

}

void AGameManager::nextRound()
{
	++roundCount;
}
