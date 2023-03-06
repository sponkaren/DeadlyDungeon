// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"	
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "MenuManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCreated, APlayerCharacter*, character);

UCLASS()
class DEADLYDUNGEON_API AMenuManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AActor*> foundHex;

	AHexTile* hexTile;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> playerCharacter;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> playerWarrior;

	UPROPERTY(EditAnywhere, Category = "Player Management")
	TSubclassOf<APlayerCharacter> playerRanger;

	FRotator Rotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnCharacterCreated CharacterCreated;

	int m_ID;

	void setHexDelegates();

	void spawnAlivePlayers(TArray<FPlayerStruct>& players);

	void showLastDungeonUnlock(int unlock);

	UFUNCTION()
	void whenHexClicked(AHexTile* hex);

	void getHex();
};
