// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "HexGridManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThisHexClicked, AHexTile*, hex);

/*
USTRUCT(BlueprintType)
struct FHexStruct
{
	GENERATED_BODY()
public:
	UPROPERTY()
		uint8 characterClass;
	UPROPERTY()
		int initiative;
	UPROPERTY()
		int movement;
};
*/

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

	bool debugMode{ false };

	TArray<AHexTile*> HexGridArray;

	TArray<int> hexIndexes;
	
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")

	FOnThisHexClicked thisHexClicked;

	UFUNCTION()
	void whenHexClicked(AHexTile* hex);

	void setAdjacentHex();

	bool checkIfAdjacent(AHexTile* h1, AHexTile* h2, int range=1);
	void highlightTiles(int hexIndex, int movement);
	void highlightAttackTiles(int hexIndex, int range=1);
	void highlightMovement(const TArray<int>& movementArray, bool on=true);
	void highlightsOff();
	int getNextPlayerSpawn(int players);
	int getNextEnemySpawn(int enemies);
	AHexTile* findHexByAxial(int Q, int R);

	int findClosestTarget(int hexIndex, const TArray<int>& targets, int range=1, bool highlighting=false, int movementLeft=0);
	int findClosestRangeTarget(int hexIndex, const TArray<int>& targets, int range = 1);

	bool calculateMovement(TArray<int>& movementArray, int targetHex, int hexIndex, int movementLeft, int range=1, bool highlighting=false);
	void calculateMoveTowards(TArray<int>& movementArray, int targetHex, int hexIndex, int movementLeft, int range = 1);

	void movementCalc(TArray<int>& movementArray, int hexIndex, int movementLeft, int& locPrio, int range=1, bool highlighting=false);

	int getAttractiveness(AHexTile* start, AHexTile* next, AHexTile* end);
	int getGStar(AHexTile* ref, AHexTile* end);

	void setPriorities(int targetHex, int locHex, int range=1, bool rangeCheck=false);
	void clearPriorities();

	void sortByGStar(int startIndex);
	void setIndexArray();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
