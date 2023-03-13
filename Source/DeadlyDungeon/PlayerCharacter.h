// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CharacterProjectile.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharClicked, APlayerCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharIdle, APlayerCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharShot, APlayerCharacter*, character);

USTRUCT(BlueprintType)
struct FPlayerStruct
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int ID;
	UPROPERTY()
	uint8 characterClass;
	UPROPERTY()
	int initiative;
	UPROPERTY()
	int movement;
	UPROPERTY()
	float attack;
	UPROPERTY()
	int numberOfAttacks;
	UPROPERTY()
	float maxHealth;
	UPROPERTY()
	int range;
};

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	OTTO,
	WARRIOR,
	HEALER,
	MAGE,
	RANGER,
	MAX UMETA(Hidden)
};


UENUM()
enum class CharacterType : uint8
{
	ALLY, 
	ENEMY,
	MAX UMETA (Hidden)
};

constexpr float baseWait{ 0.1 };

UCLASS()
class DEADLYDUNGEON_API APlayerCharacter : public APawn
{
	GENERATED_BODY()
	

public:

	enum e_state
	{
		IDLE, 
		MOVING,
		ATTACKING,		
	};

	e_state m_state{ IDLE };

	//personal
	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_ID;

	UPROPERTY(BlueprintType)
	ECharacterClass m_characterClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_initiative;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_movement;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	float m_attack;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_numberOfAttacks;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	float m_maxHealth;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_range;
	//end of stats


	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_movementLeft{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float m_currentHealth;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_numberOfAttacksLeft;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	bool AIAttack{ false };

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int AITarget{0};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	CharacterType m_type {CharacterType::ALLY};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	USkeletalMeshComponent* m_playerCharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	USceneCaptureComponent2D* sceneCaptureComponent;

	UPROPERTY(EditAnywhere, Category = "Player, HUD and UI")
	UMaterialInstanceDynamic* iconMaterial;

	UPROPERTY(EditAnywhere, Category = "Player, HUD and UI")
	UMaterialInterface* iconMaterialBase;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	//TSubclassOf<class UTextureRenderTarget2D> textureRenderTargetClass;

	UPROPERTY(EditAnywhere, Category = "Player, HUD and UI")
	UTextureRenderTarget2D* textureRenderTarget;

	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<ACharacterProjectile> m_characterProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	UStaticMeshComponent* m_arrowMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")	
	USceneComponent* m_rootComponent;

	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	int m_index{ 999 };

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

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* allyRangerMat;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* allyWarriorMat;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* enemyRangerMat;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* enemyWarriorMat;

	FVector m_origin;
	FVector m_destination;
	
	float m_timeElapsed{ 0 };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_lerpDuration{ baseWait };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_waitTime{ baseWait };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float m_waitTime2{ baseWait };

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	APlayerCharacter();

	void Init(FPlayerStruct& stats);

	void setStats(FPlayerStruct& stats);
	FPlayerStruct getStats();
	void setMaterial();

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnCharClicked CharClicked;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnCharIdle CharIdle;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnCharShot CharShot;

	void setIndex(int index);
	int getIndex();
	void setHexLocation(int hexIndex);
	int getHexLocation();
	FVector getLocation();
	void setLocation(FVector location);
	e_state getState();
	void setEnemy(bool enemy);
	bool isEnemy();
	bool rdyToAttack();
	bool getAttacking();
	float getAttack();
	bool getMoving();
	bool isIdle();
	void rotateTo(FVector location);
	void moveToHex(FVector destinationHex);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void startIdling();

	void setArrowOn(bool on);
	void setMovementLeft(int movement);
	void resetMoveAtk();
	void resetHealth();
	bool updateHealth(bool damage, float delta);
	bool isPlayer();

	void shootCorpse(const FRotator& direction);

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	float getCurrentHealthPercent();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	int getCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void gotShot();

	FORCEINLINE bool operator<(const APlayerCharacter& Other) const;

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void characterClicked();

	UFUNCTION()
	void corpseHit(ACharacterProjectile* projectile);

	void createRenderTarget();

	APlayerCharacter& getCharacter();
};
