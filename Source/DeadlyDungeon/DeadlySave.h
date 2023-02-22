// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/SaveGame.h"
#include "DeadlySave.generated.h"

/**
 * 
 */

UCLASS()
class DEADLYDUNGEON_API UDeadlySave : public USaveGame
{
	GENERATED_BODY()

public:

	UDeadlySave();
	
	//UPROPERTY(VisibleAnywhere, Category = Basic)
	PlayerStruct characterOne;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool One{ false };

	//UPROPERTY(VisibleAnywhere, Category = Basic)
	PlayerStruct characterTwo;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool Two{ false };

	//UPROPERTY(VisibleAnywhere, Category = Basic)
	PlayerStruct characterThree;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool Three{ false };

	//UPROPERTY(VisibleAnywhere, Category = Basic)
	PlayerStruct characterFour;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool Four{ false };

	TArray<PlayerStruct> deadPlayers;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int lastDungeonUnlock;
};
