// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridComponent.h"

// Sets default values for this component's properties
UHexGridComponent::UHexGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHexGridComponent::BeginPlay()
{
	Super::BeginPlay();

	int cubeCoordN{ 0 };
	int cubeCoordNE{ 0 };
	int cubeCoordNW{ 0 };

	
}


// Called every frame
void UHexGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

