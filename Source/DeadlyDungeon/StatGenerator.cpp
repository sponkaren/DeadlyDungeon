// Fill out your copyright notice in the Description page of Project Settings.


#include "StatGenerator.h"
#include "Math/UnrealMathUtility.h"
#include "RandomNameGenerator.h"
#include "UObject/Object.h"


FPlayerStruct StatGenerator::generateStats(int level, int ID)
{
	int randomStruct = FMath::RandRange(1, 2);
	int randomInit = FMath::RandRange(1, 9);
	FPlayerStruct stats;
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Rand: %i"),
	//	randomStruct));

	FString characterName = RandomNameGenerator::getRandomName();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Name: %s"),
	//	*FString(characterName)));

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
			randomInit,
			//int movement
			3,
			//float attack
			20,
			//int numberOfAttacks
			1,
			//float maxHealth
			80,
			//int range
			1,
			characterName
		};
		break;
	case 2:
		//ranger
		stats = {
			//ID
			ID,
			static_cast<uint8>(ECharacterClass::RANGER),
			//int initiative
			randomInit,
			//int movement
			3,
			//float attack
			15,
			//int numberOfAttacks
			1,
			//float maxHealth
			60,
			//int range
			2,
			characterName
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
