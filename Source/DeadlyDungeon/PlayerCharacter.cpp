// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include <cassert>
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_playerCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_arrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	sceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	m_playerCharacterMesh->SetupAttachment(m_rootComponent);
	m_arrowMesh->SetupAttachment(m_rootComponent);
	sceneCaptureComponent->SetupAttachment(m_playerCharacterMesh, "Head");	
}

void APlayerCharacter::Init(FPlayerStruct& stats)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Character init()!"));
	PrimaryActorTick.bCanEverTick = true;

	setStats(stats);
	setMaterial();
	resetMoveAtk();
	resetHealth();	
}

void APlayerCharacter::setStats(FPlayerStruct& stats)
{
	m_ID = stats.ID;
	m_characterClass = static_cast<ECharacterClass>(stats.characterClass);
	m_initiative = stats.initiative;
	m_movement = stats.movement;
	m_attack = stats.attack;
	m_numberOfAttacks = stats.numberOfAttacks;
	m_maxHealth = stats.maxHealth;
	m_range = stats.range;
	m_name = stats.name;
}

FPlayerStruct APlayerCharacter::getStats()
{
	FPlayerStruct playerStruct = {m_ID, static_cast<uint8>(m_characterClass), m_initiative, m_movement, m_attack, m_numberOfAttacks, m_maxHealth, m_range,m_name};
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("m_ID is: %i"),
	//	m_ID));
	return playerStruct;
}

void APlayerCharacter::setMaterial()
{
	//m_Material = MaterialManager::getMaterial(static_cast<int>(m_characterClass), isEnemy());

	if (isEnemy())
	{
		switch (m_characterClass)
		{
		case(ECharacterClass::WARRIOR):
			if (enemyWarriorMat)
				m_playerCharacterMesh->SetMaterial(0, enemyWarriorMat);
			break;
		case(ECharacterClass::RANGER):
			if(enemyRangerMat)
				m_playerCharacterMesh->SetMaterial(0, enemyRangerMat);
			break;
		}
	}
	else
	{
		switch (m_characterClass)
		{
		case(ECharacterClass::WARRIOR):
			if (allyWarriorMat)
				m_playerCharacterMesh->SetMaterial(0, allyWarriorMat);
			break;
		case(ECharacterClass::RANGER):
			if (allyRangerMat)
				m_playerCharacterMesh->SetMaterial(0, allyRangerMat);
			break;
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Test!"));

	if(m_state == MOVING)
	{
		if (m_waitTime > 0)
		{
			m_waitTime -= DeltaTime;
			return;
		}		
		

		if (m_timeElapsed <= m_lerpDuration)
		{
			SetActorLocation(FMath::Lerp(m_origin, m_destination, m_timeElapsed/m_lerpDuration));	
			m_timeElapsed += DeltaTime;
		}


		else if (m_waitTime2 > 0)
		{
			m_waitTime2 -= DeltaTime;
			return;
		}

		else
		{
			SetActorLocation(m_destination);
			m_timeElapsed = 0;
			m_lerpDuration = baseWait;
			m_waitTime = baseWait;
			m_waitTime2 = baseWait;			
			m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedAnim, true);
			setLocation(m_destination);
			startIdling();
		}
	}

	if (isBlasting)
	{
		if (m_blastWait > 0)
		{
			m_blastWait -= DeltaTime;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Blasting!"));
			return;
		}
		else
		{	
			isBlasting = false;			
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Blasted!"));
			BlastDone.Broadcast(m_attack);
		}
	}

	if (!isBlasting && m_blastWait <= 0)
	{
		if(m_blastWait2 > 0)
		{
			m_blastWait2 -= DeltaTime;
			return;
		}
		else
		{
			m_blastWait = 1.5;
			m_blastWait2 = 0.5;
			m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_attackAnim, true);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::setIndex(int index)
{
	m_index = index;
}

int APlayerCharacter::getIndex()
{
	return m_index;
}

void APlayerCharacter::setHexLocation(int hexIndex)
{
	m_hexLocationIndex = hexIndex;
}

int APlayerCharacter::getHexLocation()
{
	return m_hexLocationIndex;
}

FVector APlayerCharacter::getLocation()
{
	return GetActorLocation();
}

void APlayerCharacter::setLocation(FVector location)
{
	m_origin = location;
}

APlayerCharacter::e_state APlayerCharacter::getState()
{
	return m_state;
}

void APlayerCharacter::setEnemy(bool enemy)
{
	if (enemy)
	{
		m_type = CharacterType::ENEMY;
	}
	else
	{
		m_type = CharacterType::ALLY;
	}
}

bool APlayerCharacter::isEnemy()
{
	return m_type == CharacterType::ENEMY;
}

bool APlayerCharacter::rdyToAttack()
{
	if (m_numberOfAttacksLeft <= 0)
	{
		return false;
	}
	else if (m_state == IDLE)
	{
		return true;
	}
	else if (m_state == MOVING)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Can't attack! Still moving!"));
		return false;
	}
	else
	{
		return true;
	}
}

bool APlayerCharacter::getAttacking()
{
	return (m_state == ATTACKING && m_numberOfAttacksLeft > 0);
}

float APlayerCharacter::getAttack()
{
	return m_attack;
}

bool APlayerCharacter::getMoving()
{
	return m_state == MOVING;
}

bool APlayerCharacter::isIdle()
{
	return m_state == IDLE;
}

void APlayerCharacter::rotateTo(FVector destination)
{
	FRotator rotation{ UKismetMathLibrary::FindLookAtRotation(m_origin, destination)};
	SetActorRotation(rotation);
}

void APlayerCharacter::moveToHex(FVector destinationHex)
{	
	if (m_movementLeft > 0)
	{
		m_movementLeft -= 1;
		rotateTo(destinationHex);
		m_destination = destinationHex;
		m_state = MOVING;
		m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_jumpAnim, false);
	}
}

void APlayerCharacter::startIdling()
{
	CharIdle.Broadcast(this);
}

void APlayerCharacter::setArrowOn(bool on)
{
	m_arrowMesh->SetVisibility(on, true);
	arrowOn = on;
}

void APlayerCharacter::setMovementLeft(int movement)
{
	m_movementLeft = movement;
}

void APlayerCharacter::resetMoveAtk()
{
	m_movementLeft = m_movement;
	m_numberOfAttacksLeft = m_numberOfAttacks;
	AIAttack = false;
}

void APlayerCharacter::resetHealth()
{
	m_currentHealth = m_maxHealth;
}

bool APlayerCharacter::updateHealth(bool damage, float delta)
{
	if (damage)
	{
		m_currentHealth -= delta;
	}
	else
	{
		m_currentHealth += delta;
	}

	return m_currentHealth <= 0;
}

float APlayerCharacter::getCurrentHealthPercent()
{
	return m_currentHealth/m_maxHealth;
}

int APlayerCharacter::getCurrentHealth()
{
	return m_currentHealth;
}

void APlayerCharacter::shootCorpse(const FRotator& direction)
{	
	FName SpineBone = "Spine01";
	FVector BoneLocation = m_playerCharacterMesh->GetBoneLocation(SpineBone, EBoneSpaces::WorldSpace);
	
	if (BoneLocation != FVector(0, 0, 0))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Bone location set!"));
	}

	ACharacterProjectile* projectile = GetWorld()->SpawnActor<ACharacterProjectile>(m_characterProjectile, BoneLocation, GetActorRotation());

	if (projectile)
	{
		projectile->ProjectileOverlap.AddDynamic(this, &APlayerCharacter::corpseHit);
	}

	if (PhysicsHandle == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Handler not found!"));
	}

	if (PhysicsHandle)
	{
		PhysicsHandle->GrabComponentAtLocation(m_playerCharacterMesh, SpineBone, BoneLocation);
		projectile->shoot(direction, PhysicsHandle);
	}

}

void APlayerCharacter::corpseHit(ACharacterProjectile* projectile)
{
	if (PhysicsHandle)
	{
		PhysicsHandle->ReleaseComponent();	
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Handler deactivate!"));
	}
}

void APlayerCharacter::gotShot()
{
	if (m_currentHealth > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Shot character ID: %i"),
		//	m_hexLocationIndex));

		CharShot.Broadcast(this);
	}
}

bool APlayerCharacter::operator<(const APlayerCharacter& Other) const
{
	return m_initiative > Other.m_initiative;
}

void APlayerCharacter::characterClicked()
{
	CharClicked.Broadcast(this);
}

void APlayerCharacter::characterSelected()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Clicked!"));
	CharSelect.Broadcast(this);
}

void APlayerCharacter::createRenderTarget()
{
	//Creating render target
	textureRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld());
	
	//Setting render target of the scene capture component
	if(textureRenderTarget)
	{
		sceneCaptureComponent->TextureTarget = textureRenderTarget;
	}

	iconMaterial = UMaterialInstanceDynamic::Create(iconMaterialBase, this);
	iconMaterial->SetTextureParameterValue("TextureParam", textureRenderTarget);
}

void APlayerCharacter::setBlasting()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Set to Blast!"));
	m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_blastAnim, false);
	isBlasting = true;
}

const FString& APlayerCharacter::getName()
{
	return m_name;
}


APlayerCharacter& APlayerCharacter::getCharacter()
{
	return *this;
}
