// Fill out your copyright notice in the Description page of Project Settings.


#include "EndScreenWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>


void UEndScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();	
	
	returnButton->OnClicked.AddDynamic(this, &UEndScreenWidget::OnToMenuButtonClicked);
}

void UEndScreenWidget::setText(bool win)
{
	if(win)
	{ 
		winLoseText->SetText(winText);
	}
	else
	{
		winLoseText->SetText(loseText);
	}
}

void UEndScreenWidget::OnToMenuButtonClicked()
{
	ToMenu.Broadcast();
}