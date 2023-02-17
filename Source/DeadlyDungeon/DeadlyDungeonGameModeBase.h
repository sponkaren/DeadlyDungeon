// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerManager.h"
#include "HexGridManager.h"
#include "GameFramework/GameModeBase.h"
#include "DeadlyDungeonGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API ADeadlyDungeonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "DungeonMode")
	TSubclassOf<APlayerManager> playerManager;

	UPROPERTY(EditAnywhere, Category = "DungeonMode")
	TSubclassOf<AHexGridManager> hexManager;

};
