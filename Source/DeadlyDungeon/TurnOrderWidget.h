// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitIconWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "TurnOrderWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API UTurnOrderWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	TArray<UUnitIconWidget*> iconArray;
	
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* verticalBox;

public:
	void createUnitIcon(UMaterialInterface* texture);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	TSubclassOf<class UUnitIconWidget> iconWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	class UUnitIconWidget* iconWidget;
};
