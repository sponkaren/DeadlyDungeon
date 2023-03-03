// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterProjectile.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACharacterProjectile::ACharacterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	m_projectile->SetupAttachment(m_rootComponent);

}

// Called when the game starts or when spawned
void ACharacterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (go)
	{
		if (m_timeElapsed < m_lerpDuration)
		{
			SetActorLocation(FMath::Lerp(m_origin, m_destination, m_timeElapsed / m_lerpDuration));
			m_timeElapsed += DeltaTime;
			m_handler->SetTargetLocation(GetActorLocation());			
		}
		else
		{
			ProjectileOverlap.Broadcast(this);
			Destroy();
		}
	}

}

void ACharacterProjectile::shoot(FRotator direction, UPhysicsHandleComponent* handler)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("YEET!"));

	m_handler = handler;
	m_direction = direction;
	m_lerpDuration = 5;
	m_timeElapsed = 0;
	m_origin = GetActorLocation();
	m_destination = m_origin + direction.Vector() * -5000;
	go = true;
}

bool ACharacterProjectile::hitSomeone(int health)
{
	if (health > 0)
	{
		ProjectileOverlap.Broadcast(this);
		go = false;		
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("We continue!"));
		return false;
	}
}

void ACharacterProjectile::destroyProjectile()
{
	Destroy();
}


