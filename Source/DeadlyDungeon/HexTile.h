// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Containers/UnrealString.h>
#include "HexTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHexClicked, AHexTile*, hex);

UENUM()
enum class EHexTileType : uint8
{
	INVALID,
	GRASS,
	WATER,
	MENU,
	MAX UMETA(Hidden)
};

UCLASS()
class DEADLYDUNGEON_API AHexTile : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		UStaticMeshComponent* m_tileMesh {};


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		UStaticMeshComponent* m_tileHighlight {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		UStaticMeshComponent* m_tileAttackHighlight {};

	USceneComponent* m_rootComponent;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	EHexTileType m_tileType {};

	UPROPERTY(VisibleInstanceOnly, Category = "Tile");
	bool m_occupied{};

	UPROPERTY(VisibleInstanceOnly, Category = "Tile");
	FVector m_location{};

	UPROPERTY(VisibleInstanceOnly, Category = "Tile");
	int m_axialR{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tile");
	int m_axialQ{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tile");
	int m_axialS{};

	UPROPERTY(VisibleInstanceOnly, Category = "Tile");
	int m_index{};

	// Sets default values for this actor's properties
	AHexTile();

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnHexClicked HexTileClicked;

	void setAxial(int r, int q);
	int getAxialR();
	int getAxialQ();
	int getAxialS();
	
	void setIndex(int);

	void setLocation(FVector);
	
	FVector getLocation();

	void setOccupied(bool);
	bool getOccupied();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void hexClicked();

	void setHighightVisible(bool on);

	void setAttackHighightVisible(bool on);

	int getDistance(const AHexTile& refHex);

	AHexTile& getHex();
};
