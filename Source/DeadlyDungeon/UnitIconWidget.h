// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UnitIconWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIconHover, UUnitIconWidget*, icon, bool, on);
/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API UUnitIconWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* ImageOn;

public:
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnIconHover IconHovered;

	void setImage(UMaterialInterface* texture);

	void activeTurn(bool on);

	UFUNCTION(BlueprintCallable, Category = "Player, HUD and UI")
	void onIconHover(bool on);
};
