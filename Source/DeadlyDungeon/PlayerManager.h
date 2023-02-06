// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
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

	UPROPERTY(EditAnywhere, Category = "Player Management")
		int numberOfCharacters;

	UPROPERTY(EditAnywhere, Category = "Player Management")
		TSubclassOf<APlayerCharacter> m_characterToSpawn;

	UPROPERTY(EditAnywhere, Category = "Player Management")
		TSubclassOf<APlayerCharacter> m_playerCharacter;

	UPROPERTY(EditAnywhere, Category = "Player Management")
		TSubclassOf<APlayerCharacter> m_enemyCharacter;

	static TArray<APlayerCharacter*> CharacterArray;

	static APlayerCharacter* m_selectedCharacter;

	int maxPlayers{ 100 };

	static int turnIndex;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void spawnPlayer(int hexIndex, bool enemy);

	void static storeSelectedCharacter(APlayerCharacter*);

	static APlayerCharacter* getSelectedCharacer();

	static void movePlayerCharacter(int destinationHexIndex);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
		static void sortByInitiative();

	static int getnextTurn();

	bool greaterInitiative(APlayerCharacter& c1, APlayerCharacter& c2);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void setNextTurn();
};
