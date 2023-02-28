// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include <cassert>
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"

//constexpr float baseWait{ 0.5 };

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_playerCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_arrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	m_playerCharacterMesh->SetupAttachment(m_rootComponent);
	m_arrowMesh->SetupAttachment(m_rootComponent);
}

void APlayerCharacter::Init(FPlayerStruct& stats)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Character init()!"));
	PrimaryActorTick.bCanEverTick = true;

	setStats(stats);
	resetMoveAtk();
	resetHealth();	
}

void APlayerCharacter::setStats(FPlayerStruct& stats)
{
	m_characterClass = stats.characterClass;
	m_initiative = stats.initiative;
	m_movement = stats.movement;
	m_numberOfAttacks = stats.numberOfAttacks;
	m_attack = stats.attack;
	m_maxHealth = stats.maxHealth;
	m_range = stats.range;
}

FPlayerStruct APlayerCharacter::getStats()
{
	FPlayerStruct playerStruct = { m_characterClass, m_initiative, m_movement, m_numberOfAttacks, m_attack, m_maxHealth, m_range };
	
	return playerStruct;
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
		

		if (m_timeElapsed < m_lerpDuration)
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
			m_timeElapsed = 0;
			m_lerpDuration = baseWait;
			m_waitTime = baseWait;
			m_waitTime2 = baseWait;			
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Can't attack! Still moving!"));
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

bool APlayerCharacter::operator<(const APlayerCharacter& Other) const
{
	return m_initiative > Other.m_initiative;
}

void APlayerCharacter::characterClicked()
{
	CharClicked.Broadcast(this);
}

APlayerCharacter& APlayerCharacter::getCharacter()
{
	return *this;
}
