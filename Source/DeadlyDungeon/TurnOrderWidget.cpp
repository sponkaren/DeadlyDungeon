// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnOrderWidget.h"

void UTurnOrderWidget::createUnitIcon(UMaterialInterface*texture)
{

    iconWidget = CreateWidget<UUnitIconWidget>(this, iconWidgetClass);
	iconWidget->setImage(texture);
	verticalBox->AddChild(iconWidget);	
	iconArray.Emplace(iconWidget);
	iconWidget->IconHovered.AddDynamic(this, &UTurnOrderWidget::iconHovered);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Adding Icon!"));
}

void UTurnOrderWidget::setActiveTurn(int index)
{
	if (iconArray.Num() > index)
	{
		for (UUnitIconWidget* icon : iconArray)
		{
			icon->activeTurn(false);
		}			
		iconArray[index]->activeTurn(true);		
	}
}

void UTurnOrderWidget::iconHovered(UUnitIconWidget* icon, bool on)
{
	int i{ 0 };
	for (UUnitIconWidget* possibleIcon : iconArray)
	{
		if (possibleIcon == icon)
			HoverChange.Broadcast(i, on);
		else
			++i;
	}
}

void UTurnOrderWidget::deleteIcon(int index)
{
	iconArray[index]->RemoveFromParent();
	iconArray.RemoveAt(index);
}