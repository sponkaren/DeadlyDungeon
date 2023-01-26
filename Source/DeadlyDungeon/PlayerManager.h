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
		int numberOfPlayers;

	UPROPERTY(EditAnywhere, Category = "Player Management")
		TSubclassOf<APlayerCharacter> m_playerCharacter;

	TArray<APlayerCharacter*> PlayerArray;

	int maxPlayers{ 100 };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void spawnPlayer(int hexIndex);

};
