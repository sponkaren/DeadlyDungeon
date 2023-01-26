// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
class DEADLYDUNGEON_API APlayerCharacter : public APawn
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		PlayerCharacterType m_playerCharacterType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		USkeletalMeshComponent* m_playerCharacterMesh;

	USceneComponent* m_rootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		AHexTile* hexLocation {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		FRotator charRotation {};


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	APlayerCharacter();

	void setLocation(AHexTile&);

	void rotateToHex(const AHexTile&);
	void moveToHex(const AHexTile&);

	
};
