// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerManager.h"
#include "HexTile.h"
#include "HexGridManager.h"
#include <cassert>
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

APlayerCharacter* APlayerCharacter::m_lastClicked{ nullptr };
constexpr float baseWait{ 0.5 };

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_playerCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_arrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	m_playerCharacterMesh->SetupAttachment(m_rootComponent);
	m_arrowMesh->SetupAttachment(m_rootComponent);
	m_lastClicked = nullptr;
	m_timeElapsed = 0;
	m_lerpDuration = baseWait;
	m_waitTime = baseWait;
	m_waitTime2 = baseWait;
	m_state = IDLE;

	//Stats
	m_initiative = FMath::RandRange(1, 9);
	m_movement = 3;
	resetMovement();
	m_attack = 10;
	m_maxHealth = 100;
	resetHealth();
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
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Waiting!"));
			m_waitTime -= DeltaTime;
			return;
		}		
		

		if (m_timeElapsed < m_lerpDuration)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Moving!"));
			SetActorLocation(FMath::Lerp(m_origin, m_destination, m_timeElapsed/m_lerpDuration));	
			m_timeElapsed += DeltaTime;
		}


		else if (m_waitTime2 > 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Waiting again!"));
			m_waitTime2 -= DeltaTime;
			return;
		}

		else
		{
			m_timeElapsed = 0;
			m_lerpDuration = baseWait;
			m_waitTime = baseWait;
			m_waitTime2 = baseWait;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Stop moving!"));			
			m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedAnim, true);
			setLocation(m_destination);
			startIdling();
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

void APlayerCharacter::setAttacking()
{
	m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_attackAnim, true);
	m_state = ATTACKING;
	AHexGridManager::highlightAttackTiles(getHexLocation());
}

bool APlayerCharacter::getAttacking()
{
	return m_state == ATTACKING;
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

		//SetActorLocation(destinationHex);
	}
}

void APlayerCharacter::setSelectedCharacter()
{	
	if (IsValid(m_lastClicked))
	{
		m_lastClicked->setMovementLeft(0);
		m_lastClicked->startIdling();
		m_lastClicked->setArrowOn(false);
		AHexGridManager::highlightsOff();
	}
	m_lastClicked = this;
	setArrowOn(true);
	resetMovement();
	m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedAnim, true);
	APlayerManager::storeSelectedCharacter(m_lastClicked);
	AHexGridManager::highlightTiles(getHexLocation());

}

void APlayerCharacter::startIdling()
{
	AHexGridManager::highlightsOff();
	m_state = IDLE;

	if (m_movementLeft > 0)
	{
		m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedAnim, true);
		AHexGridManager::highlightTiles(getHexLocation());
	}
	else
	{
		m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_idleAnim, true);
	}
	
}

void APlayerCharacter::setArrowOn(bool on)
{
	m_arrowMesh->SetVisibility(on, true);
}

int APlayerCharacter::getInitiative()
{
	return m_initiative;
}

int APlayerCharacter::getMovementLeft()
{
	return m_movementLeft;
}

void APlayerCharacter::setMovementLeft(int movement)
{
	m_movementLeft = movement;
}

void APlayerCharacter::resetMovement()
{
	m_movementLeft = m_movement;
}

void APlayerCharacter::resetHealth()
{
	m_currentHealth = m_maxHealth;
}

void APlayerCharacter::updateHealth(bool damage, float delta)
{
	if (damage)
	{
		m_currentHealth -= delta;
	}
	else
	{
		m_currentHealth += delta;
	}
}

float APlayerCharacter::getCurrentHealth()
{
	return m_currentHealth;
}

float APlayerCharacter::getCurrentHealthPercent()
{
	return m_currentHealth/m_maxHealth;
}

bool APlayerCharacter::operator<(const APlayerCharacter& Other) const
{
	return m_initiative > Other.m_initiative;
}

void APlayerCharacter::characterClicked()
{
	APlayerManager::characterClicked(getCharacter());
}

APlayerCharacter& APlayerCharacter::getCharacter()
{
	return *this;
}
