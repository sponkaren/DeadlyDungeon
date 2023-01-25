// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class PlayerCharacterType : uint8
{
	OTTO,
	WARRIOR,
	MAGE,
	MAX UMETA(Hidden)
};

UCLASS()
class DEADLYDUNGEON_API APlayerCharacter : public AActor
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		PlayerCharacterType m_playerCharacterType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		UStaticMeshComponent* m_playerCharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		AHexTile* hexLocation {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		FRotator charRotation {};


public:	
	APlayerCharacter();

	void setLocation(AHexTile&);

	void rotateToHex(const AHexTile&);
	void moveToHex(const AHexTile&);

	
};
