// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerManager.h"
#include "Components/SkeletalMeshComponent.h"

APlayerCharacter* APlayerCharacter::m_lastClicked{ nullptr };

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_playerCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_playerCharacterMesh->SetupAttachment(m_rootComponent);
	m_lastClicked = nullptr;
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::setLocation(AHexTile& hex)
{
	hexLocation = &hex;
}

void APlayerCharacter::rotateToHex(const AHexTile& hex)
{

}

void APlayerCharacter::moveToHex(const AHexTile& hex)
{

}

void APlayerCharacter::setSelectedCharacter()
{	
	if (IsValid(m_lastClicked))
	{
		m_lastClicked->startIdling();
	}

	m_lastClicked = this;

	m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_selectedAnim, true);
	APlayerManager::storeSelectedCharacter(m_lastClicked);

}

void APlayerCharacter::startIdling()
{
	m_playerCharacterMesh->USkeletalMeshComponent::PlayAnimation(m_idleAnim, true);
}
