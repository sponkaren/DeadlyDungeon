// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "AxialPixelConversion.h"
#include "HexGridManager.generated.h"

UCLASS()
class DEADLYDUNGEON_API AHexGridManager : public AActor
{
	GENERATED_BODY()

protected:

	TArray<TArray<AHexTile*>>  HexGrid2DArray;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
		int32 m_gridWidth;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
		int32 m_gridHeight;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
		TSubclassOf<AHexTile> m_grassHexTile;

	UPROPERTY(EditAnywhere, Category = "HexGrid Layout")
		TSubclassOf<AHexTile> m_waterHexTile;

public:	
	// Sets default values for this actor's properties
	AHexGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
