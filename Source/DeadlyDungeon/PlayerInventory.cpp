// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"
#include "PlayerCharacter.h"

TArray<APlayerCharacter> APlayerInventory::PlayerCharacterInventory{};

// Sets default values
APlayerInventory::APlayerInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerInventory::addPlayerCharacter(FVector spawn)
{

	FRotator Rotation{ 0,0,0 };
	UWorld* world = GEngine->GameViewport->GetWorld();

	//FindObject<APlayerInventory>
	//TSubclassOf<APlayerCharacter> player{};

	//APlayerCharacter* newPlayer = world->SpawnActor<APlayerCharacter>
		//(player, spawn, Rotation);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Spawning new player character!"));

}



