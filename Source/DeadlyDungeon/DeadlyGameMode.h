// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadlyDungeonGameModeBase.h"
#include "PlayerManager.h"
#include "HexGridManager.h"
#include "DeadlyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API ADeadlyGameMode : public ADeadlyDungeonGameModeBase
{
	GENERATED_BODY()	
protected:
	UPROPERTY(EditAnywhere, Category = "DungeonMode")
	TSubclassOf<APlayerManager> bp_playerManager;

	UPROPERTY(EditAnywhere, Category = "DungeonMode")
	TSubclassOf<AHexGridManager> bp_hexManager;

public:

};
