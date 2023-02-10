// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class DEADLYDUNGEON_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	
	static int roundCount;

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	static void startGame();

	static void endGame(bool victory);

	static void nextRound();
};
