// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadlyDungeonGameModeBase.h"
#include "MenuManager.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API AMenuGameMode : public ADeadlyDungeonGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "MenuMode")
	TSubclassOf<AMenuManager> menuManager;

};
