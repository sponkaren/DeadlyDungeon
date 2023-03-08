// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnActionWidget.h"
#include <Components/Button.h>

void UTurnActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ExecuteButton->OnClicked.AddDynamic(this, &UTurnActionWidget::OnExecuteButtonClicked);
	NextTurnButton->OnClicked.AddDynamic(this, &UTurnActionWidget::OnNextTurnButtonClicked);
}

void UTurnActionWidget::OnExecuteButtonClicked()
{
	Execute.Broadcast(executeActive);
}

void UTurnActionWidget::OnNextTurnButtonClicked()
{
	NextTurn.Broadcast();
}