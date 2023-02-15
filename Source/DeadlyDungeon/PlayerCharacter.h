// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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

	enum e_state
	{
		IDLE, 
		MOVING,
		ATTACKING,		
	};

	e_state m_state;

	//STATS!!!
	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_initiative;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_movement;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_movementLeft{};

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	float m_attack;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	float m_maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float m_currentHealth;

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
	int m_index;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_hexLocationIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	FRotator m_charRotation;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* m_idleAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* m_selectedAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* m_jumpAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* m_attackAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* m_TPose;

	static APlayerCharacter* m_lastClicked;

	FVector m_origin;
	FVector m_destination;
	float m_timeElapsed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_lerpDuration;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_waitTime;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_waitTime2;





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	APlayerCharacter();

	void setIndex(int index);
	int getIndex();
	void setHexLocation(int hexIndex);
	int getHexLocation();
	FVector getLocation();
	void setLocation(FVector location);
	e_state getState();
	void setEnemy(bool enemy);
	bool isEnemy();
	bool setAttacking();
	bool getAttacking();
	float getAttack();
	bool getMoving();
	bool isIdle();
	void rotateTo(FVector location);
	void moveToHex(FVector destinationHex);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void setSelectedCharacter();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void startIdling();

	void setArrowOn(bool on);
	int getInitiative();
	int getMovementLeft();
	void setMovementLeft(int movement);
	void resetMovement();
	void resetHealth();
	void updateHealth(bool damage, float delta);
	void killMe();
	bool isPlayer();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	float getCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
		float getCurrentHealthPercent();

	FORCEINLINE bool operator<(const APlayerCharacter& Other) const;

	UFUNCTION(BlueprintCallable, Category = "Player Management")
		void characterClicked();

	APlayerCharacter& getCharacter();
};
