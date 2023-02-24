// Fill out your copyright notice in the Description page of Project Settings.


#include "StatGenerator.h"

FPlayerStruct StatGenerator::generateStats(int level)
{
	FPlayerStruct stats = {
		//int initiative;
		5,
		//int movement;
		3,
		//float attack;
		10,
		//int numberOfAttacks;
		1,
		//float maxHealth;
		100
	};

	return stats;
}

StatGenerator::StatGenerator()
{
}

StatGenerator::~StatGenerator()
{
}
