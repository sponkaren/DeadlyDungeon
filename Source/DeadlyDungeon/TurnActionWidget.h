// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnActionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecuteClicked, bool, active);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextTurnClicked);

/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API UTurnActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnExecuteClicked Execute;
	
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnNextTurnClicked NextTurn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExecuteButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExecuteLabel;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextTurnButton;

	bool nextTurnActive{ true };

	bool executeActive{ true };

	UFUNCTION(BlueprintCallable, Category="Button Commands")
	void OnExecuteButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Button Commands")
	void OnNextTurnButtonClicked();

	void NativeConstruct() override;
};
