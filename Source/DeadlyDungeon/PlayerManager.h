// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "HexGridManager.h"
#include "TurnActionWidget.h"
#include "TurnOrderWidget.h"
#include "UnitInfoWidget.h"
#include "PlayerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, int, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, win);

UCLASS()
class DEADLYDUNGEON_API APlayerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int numberOfCharacters;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> m_characterToSpawn;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> m_playerCharacter;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> m_enemyCharacter;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> m_playerWarrior;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> m_playerRanger;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> m_enemyWarrior;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> m_enemyRanger;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<AHexGridManager> bp_hexManager;

	AHexGridManager* hexManager;

	UTurnActionWidget* turnActionWidget;

	UTurnOrderWidget* turnOrderWidget;

	UUnitInfoWidget* unitInfoWidget;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	UMaterialInterface* iconMaterial;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	UTexture2D* iconTexture;

	TArray<APlayerCharacter*> CharacterArray;

	APlayerCharacter* m_selectedCharacter;

	APlayerCharacter* lastClicked;

	APlayerCharacter* populator;

	APlayerCharacter* attackedCharacter;

	int maxPlayers{ 100 };

	int turnIndex;

	TArray<int> characterMovement;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool debugMode{ false };

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnPlayerDeath PlayerDeath;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnGameOver GameOver;

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void spawnHexGridManager();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void turnActionWidgetSetup(UTurnActionWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void turnOrderWidgetSetup(UTurnOrderWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void unitInfoWidgetSetup(UUnitInfoWidget* widget);

	void setTurnOrderIcons();

	void addIconTurnOrder(UMaterialInterface* characterIcon, APlayerCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void handlePlayersToSpawn(TArray<FPlayerStruct>& players);

	void spawnEnemies(int difficulty, int numberOfEnemies);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void spawnPlayer(FPlayerStruct& stats, int hexIndex, bool enemy);

	void storeSelectedCharacter(APlayerCharacter*);

	void setSelectedCharacter(APlayerCharacter* character);
	
	void startAI();

	void movePlayerCharacter(int destinationHexIndex);

	void moveEnemy(int movement);

	void occupiedHexClicked(int hexIndex);

	UFUNCTION()
	void characterClicked(APlayerCharacter* character);

	void characterAttacked(APlayerCharacter* character);

	UFUNCTION()
	void dealDamage(float attack);

	void removeCharacter(APlayerCharacter& character, bool shot=false);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void sortByInitiative();

	void setIndexes();

	int getnextTurn();

	void checkGameOver();
	
	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void setNextTurn();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	bool setAttacking(bool ally=false);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void selectedIdle(bool ally=false);

	UFUNCTION()
	void executeActions(APlayerCharacter* character);

	UFUNCTION()
	void characterShot(APlayerCharacter* character);

	UFUNCTION()
	void whenHexClicked(AHexTile* hex);

	UFUNCTION()
	void executeClicked(bool active);

	bool validateMovement(int hexIndex);

	bool validateAttack(int hexIndex);

	void resetCommands();

	void setDebugMode(bool on);

	UFUNCTION()
	void hoverChange(int index, bool on);

	UFUNCTION()
	void populateInfo(APlayerCharacter* character);
};
