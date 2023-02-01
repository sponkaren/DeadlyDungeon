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

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
		int m_hexLocationIndex {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		FRotator m_charRotation {};

	UPROPERTY(EditDefaultsOnly)
		UAnimationAsset* m_idleAnim;

	UPROPERTY(EditDefaultsOnly)
		UAnimationAsset* m_selectedAnim;

	UPROPERTY(EditDefaultsOnly)
		UAnimationAsset* m_jumpAnim;

		static APlayerCharacter* m_lastClicked;

		bool m_isMoving{};

		FVector m_origin{};

		FVector m_destination{};

		float m_timeElapsed{};

		UPROPERTY(EditAnywhere, Category = "Movement")
			float m_lerpDuration{};

		UPROPERTY(EditAnywhere, Category = "Movement")
			float m_waitTime{};

		UPROPERTY(EditAnywhere, Category = "Movement")
			float m_waitTime2{};


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	APlayerCharacter();

	void setHexLocation(int hexIndex);

	int getHexLocation();

	void setLocation(FVector location);

	bool getMoving();

	void rotateTo(FVector location);
	
	void moveToHex(FVector destinationHex);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void setSelectedCharacter();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void startIdling();
};
