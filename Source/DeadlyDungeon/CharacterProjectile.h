// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CharacterProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileOverlap, ACharacterProjectile*, projectile);

UCLASS()
class DEADLYDUNGEON_API ACharacterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USceneComponent* m_rootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* m_projectile;

	UPhysicsHandleComponent* m_handler;

	FRotator m_direction;

	FVector m_origin;

	FVector m_destination;

	float m_timeElapsed;

	float m_lerpDuration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool go{ false };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnProjectileOverlap ProjectileOverlap;	

	void shoot(FRotator direction, UPhysicsHandleComponent* handler);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	bool hitSomeone(int health);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void destroyProjectile();
};
