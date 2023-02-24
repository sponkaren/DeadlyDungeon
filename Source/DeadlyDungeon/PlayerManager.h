// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "HexGridManager.h"
#include "PlayerManager.generated.h"

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
	TSubclassOf<AHexGridManager> bp_hexManager;

	AHexGridManager* hexManager;

	TArray<APlayerCharacter*> CharacterArray;

	APlayerCharacter* m_selectedCharacter;

	APlayerCharacter* lastClicked;

	int maxPlayers{ 100 };

	int turnIndex;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void spawnHexGridManager();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void handlePlayersToSpawn(TArray<FPlayerStruct>& players);

	void spawnEnemies(int difficulty, int numberOfEnemies);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void spawnPlayer(FPlayerStruct& stats, int hexIndex, bool enemy);

	void storeSelectedCharacter(APlayerCharacter*);

	void setSelectedCharacter(APlayerCharacter* character);

	APlayerCharacter* getSelectedCharacer();
	
	void startAI();

	void movePlayerCharacter(int destinationHexIndex);

	void moveEnemy(const TArray<int>& movements);

	void occupiedHexClicked(int hexIndex);

	UFUNCTION()
	void characterClicked(APlayerCharacter* character);

	void removeCharacter(APlayerCharacter& character);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void sortByInitiative();

	void setIndexes();

	int getnextTurn();

	void checkGameOver();
	
	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void setNextTurn();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	bool setAttacking();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void selectedIdle();

	UFUNCTION()
	void setIdle(APlayerCharacter* character);

	UFUNCTION()
	void whenHexClicked(AHexTile* hex);

	bool validateMovement(int hexIndex);

	bool validateAttack(int hexIndex);

};
