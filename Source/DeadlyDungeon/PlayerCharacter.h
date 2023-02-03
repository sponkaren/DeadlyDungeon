// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HexTile.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class PlayerCharacterClass : uint8
{
	OTTO,
	WARRIOR,
	MAGE,
	MAX UMETA(Hidden)
};

UENUM()
enum class CharacterType : uint8
{
	ALLY, 
	ENEMY,
	MAX UMETA (Hidden)
};

UCLASS()
class DEADLYDUNGEON_API APlayerCharacter : public APawn
{
	GENERATED_BODY()
	

protected:

	//STATS!!!
	UPROPERTY(VisibleInstanceOnly, Category = "Player")
		int m_initiative{};


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		PlayerCharacterClass m_playerCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		CharacterType m_type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		USkeletalMeshComponent* m_playerCharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		UStaticMeshComponent* m_arrowMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		

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

	void setArrowOn(bool on);

	int getInitiative();

	FORCEINLINE bool operator<(const APlayerCharacter& Other) const;
};
