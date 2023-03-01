// Fill out your copyright notice in the Description page of Project Settings.


#include "StatGenerator.h"
#include "Math/UnrealMathUtility.h"

FPlayerStruct StatGenerator::generateStats(int level, int ID)
{
	int randomStruct = FMath::RandRange(1, 2);
	
	FPlayerStruct stats;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Rand: %i"),
		randomStruct));

	switch (randomStruct)
	{
	case 1:
		//warrior
		stats = {
			//
			ID,
			//character class
			static_cast<uint8>(ECharacterClass::WARRIOR),
			//int initiative
			5,
			//int movement
			3,
			//float attack
			10,
			//int numberOfAttacks
			1,
			//float maxHealth
			100,
			//int range
			1
		};
		break;
	case 2:
		//ranger
		stats = {
			//ID
			ID,
			static_cast<uint8>(ECharacterClass::RANGER),
			//int initiative
			5,
			//int movement
			3,
			//float attack
			6,
			//int numberOfAttacks
			1,
			//float maxHealth
			80,
			//int range
			2
		};
		break;
	}

	return stats;
}

StatGenerator::StatGenerator()
{
}

StatGenerator::~StatGenerator()
{
}
