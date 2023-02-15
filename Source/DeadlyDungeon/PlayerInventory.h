// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "PlayerInventory.generated.h"

UCLASS()
class DEADLYDUNGEON_API APlayerInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	static TArray<APlayerCharacter> PlayerCharacterInventory;

	UPROPERTY(EditAnywhere, Category = "Player Management")
		TSubclassOf<APlayerCharacter> m_characterToSpawn;

	//static TArray<APlayerCharacter> DeadPlayerCharacters;

	 

public:	

	void addPlayerCharacter(FVector spawn);

};
