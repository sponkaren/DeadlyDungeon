// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomNameGenerator.h"
#include "Math/UnrealMathUtility.h"

TArray<FString> RandomNameGenerator::firstNames = { 
	"Otto", 
	"Ivan", 
	"Neo",
	"Gronk",
	"Vasilius",
	"Ricardo",
	"Bert",
	"Sven",
	"Kurt",
	"Lonk",
	"Ferdinand",
	"Mario",
	"Bartholomew",
	"Timmy",
	"Jimmy",
	"Donut",
	"Craig",
	"Butch",
	"Xermander",
	"Father", 
	"Lord",
	"Sir",
	"Uno",
	"Sneaky"
};
TArray<FString> RandomNameGenerator::lastNames = { 
	" Botto", 
	" Stronkovic", 
	" Anderson",
	" the Unbreakable",
	" Kewling",
	" the Renlentless",
	" Knuckle",
	" the Apostle",
	"",
	" Cannonfodder",
	" Von Bert",
	" the Killer",
	" Crustyheart",
	" Birchshield",
	" the Whisperer",
	" of the Taint",
	" the Raven",
	" Seafarer",
	" of the Brew", 
	" the Untamed",
	" Son of Bert",
	" Dragonfan"
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
