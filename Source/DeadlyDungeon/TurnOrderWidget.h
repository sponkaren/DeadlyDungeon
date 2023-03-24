// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitIconWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "PlayerCharacter.h"
#include "TurnOrderWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHoverChange, int, index, bool, on);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIconClicked, APlayerCharacter*, character)
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
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnHoverChange HoverChange;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnIconClicked IconClicked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	TSubclassOf<class UUnitIconWidget> iconWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	class UUnitIconWidget* iconWidget;

	void createUnitIcon(UMaterialInterface* texture, APlayerCharacter* character);

	void setActiveTurn(int index);

	UFUNCTION()
	void iconHovered(UUnitIconWidget* icon, bool on);

	void deleteIcon(int index);

	UFUNCTION()
	void iconWasClicked(APlayerCharacter* character);
};
