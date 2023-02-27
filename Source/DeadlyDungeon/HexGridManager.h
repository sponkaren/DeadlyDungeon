// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "HexGridManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThisHexClicked, AHexTile*, hex);

UCLASS()
class DEADLYDUNGEON_API AHexGridManager : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
	int32 m_gridWidth;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
	int32 m_gridHeight;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
	int32 numberOfTiles;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
	TSubclassOf<AHexTile> m_grassHexTile;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
	TSubclassOf<AHexTile> m_waterHexTile;

public:	
	
	AHexGridManager();

	TArray<AHexTile*> HexGridArray;
	
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")

	FOnThisHexClicked thisHexClicked;

	UFUNCTION()
	void whenHexClicked(AHexTile* hex);

	bool checkIfAdjacent(AHexTile* h1, AHexTile* h2);
	void highlightTiles(int hexIndex);
	void highlightAttackTiles(int hexIndex);
	void highlightsOff();
	int getNextPlayerSpawn();
	int getNextEnemySpawn();
	AHexTile* findHexByAxial(int Q, int R);

	int findClosestTarget(int hexIndex, const TArray<int>& targets);

	void calculateMovement(TArray<int>& movementArray, int targetHex, int hexIndex, int movementLeft);

	int getAttractiveness(AHexTile* start, AHexTile* next, AHexTile* end);

	void setPriorities(int hexIndex);
	void clearPriorities();
	int getTargetPriority(int hexIndex, int targetIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
