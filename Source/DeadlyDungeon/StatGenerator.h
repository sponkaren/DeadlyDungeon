// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "CoreMinimal.h"

class DEADLYDUNGEON_API StatGenerator
{
public:

	StatGenerator();
	~StatGenerator();

	static FPlayerStruct generateStats(int level, int ID=0);
};
