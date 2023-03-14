// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreenWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnToMenuClicked);
/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	FText winText{ FText::FromString("YOU WIN!") };
	FText loseText{ FText::FromString("YOU LOSE!") };

public:

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnToMenuClicked ToMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* returnButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* winLoseText;
	
	UFUNCTION(BlueprintCallable, Category = "Button Commands")
	void OnToMenuButtonClicked();

	void setText(bool win);

	void NativeConstruct() override;
};
