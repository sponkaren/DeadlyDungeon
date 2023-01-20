// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Containers/UnrealString.h>
#include "HexTile.generated.h"



UENUM()
enum class EHexTileType : uint8
{
	INVALID,
	GRASS,
	WATER,
	MAX UMETA(Hidden)
};

UCLASS()
class DEADLYDUNGEON_API AHexTile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		EHexTileType m_tileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		UStaticMeshComponent* m_tileMesh;

	USceneComponent* m_rootComponent;

	UPROPERTY(VisibleInstanceOnly, Category = "Tile");
	int m_axialR;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tile");
	int m_axialQ;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tile");
	int m_axialS;

	UPROPERTY(VisibleInstanceOnly, Category = "Tile");
	int m_indexX;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tile");
	int m_indexY;

public:	
	// Sets default values for this actor's properties
	AHexTile();

	void setAxial(int, int);
	void setIndex(int, int);
};
