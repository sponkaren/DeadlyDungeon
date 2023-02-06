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
	m_isMoving = false;
	m_timeElapsed = 0;
	m_lerpDuration = baseWait;
	m_waitTime = baseWait;
	m_waitTime2 = baseWait;

	//Stats
	m_initiative = FMath::RandRange(1, 9);
	m_movement = 3;
	resetMovement();
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Test!"));

	if(m_isMoving)
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
			m_isMoving = false;
			m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedAnim, true);
			setLocation(m_destination);
			if (m_movementLeft > 0)
			{
				m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedAnim, true);
				AHexGridManager::highlightTiles(getHexLocation());
			}
			else
			{
				startIdling();
			}
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

void APlayerCharacter::setLocation(FVector location)
{
	m_origin = location;
}

bool APlayerCharacter::getMoving()
{
	return m_isMoving;
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
		m_isMoving = true;
		m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_jumpAnim, false);

		//SetActorLocation(destinationHex);
	}
}

void APlayerCharacter::setSelectedCharacter()
{	
	if (IsValid(m_lastClicked))
	{
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
	m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_idleAnim, true);
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

void APlayerCharacter::resetMovement()
{
	m_movementLeft = m_movement;
}

bool APlayerCharacter::operator<(const APlayerCharacter& Other) const
{
	return m_initiative > Other.m_initiative;
}
