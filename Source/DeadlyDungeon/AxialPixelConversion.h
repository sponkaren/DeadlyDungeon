// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class DEADLYDUNGEON_API AxialPixelConversion
{
public:
	static FVector axialToPixel(int r, int q);
	AxialPixelConversion();
	~AxialPixelConversion();
};
