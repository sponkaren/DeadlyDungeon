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
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int characterID{ 1 };

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FPlayerStruct> alivePlayers;
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FPlayerStruct> deadPlayers;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int lastDungeonUnlock;
};
