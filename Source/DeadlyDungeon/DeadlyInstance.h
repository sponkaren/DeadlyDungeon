// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuManager.h"
#include "PlayerManager.h"
#include "DeadlySave.h"
#include "TurnActionWidget.h"
#include "TurnOrderWidget.h"
#include "EndScreenWidget.h"
#include "UnitInfoWidget.h"
#include "DeadlyInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API UDeadlyInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	AActor* menuActor;

	AMenuManager* menuManager;

	AActor* playerActor;

	APlayerManager* playerManager;

public: 

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void MenuSetup();

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void DungeonSetup();

	UPROPERTY(BlueprintReadOnly)
	FString SaveGameSlotName;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	UDeadlySave* SaveGameObject;

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void LogIfGameWasSavedOrNot(const bool IsSaved);

	UFUNCTION()
	void SaveCharacter(APlayerCharacter* character);

	UFUNCTION()
	void DeleteCharacter(int ID);

	UFUNCTION()
	void spawnWidgets();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	TSubclassOf<class UTurnActionWidget> turnActionWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	class UTurnActionWidget* turnActionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	TSubclassOf<class UTurnOrderWidget> turnOrderWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	class UTurnOrderWidget* turnOrderWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	TSubclassOf<class UEndScreenWidget> endScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	class UEndScreenWidget* endScreenWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	TSubclassOf<class UUnitInfoWidget> unitInfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	class UUnitInfoWidget* unitInfoWidget;

	UPROPERTY(EditAnywhere)
	FName LevelToLoad;

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	bool characterExists();

	UFUNCTION()
	void setGameOver(bool win);

	UFUNCTION()
	void goToMenu();
};
