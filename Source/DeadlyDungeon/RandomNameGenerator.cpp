// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomNameGenerator.h"
#include "Math/UnrealMathUtility.h"

TArray<FString> RandomNameGenerator::firstNames = { 
	"Otto", 
	"Ida", 
	"Gustaf",
	"Ivan"
};
TArray<FString> RandomNameGenerator::lastNames = { 
	" Botto", 
	" Stronkovic", 
	" Anderson",
	" The Unbreakable",
	", Sage of Punishment"
};

RandomNameGenerator::RandomNameGenerator()
{
}

RandomNameGenerator::~RandomNameGenerator()
{
}


FString RandomNameGenerator::getRandomName()
{
	FString temp{};

	int firstInt{ FMath::RandRange(0,(firstNames.Num() - 1))};
	int lastInt{ FMath::RandRange(0,(lastNames.Num() - 1)) };
	temp += firstNames[firstInt];
	temp += lastNames[lastInt];
	
	return temp;
}
