// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEADLYDUNGEON_API RandomNameGenerator
{
public:
	RandomNameGenerator();
	~RandomNameGenerator();

	static TArray<FString> firstNames;
	static TArray<FString> lastNames;

	static FString getRandomName();

};
