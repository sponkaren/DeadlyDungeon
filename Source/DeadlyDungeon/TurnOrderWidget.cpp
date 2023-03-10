// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnOrderWidget.h"

void UTurnOrderWidget::createUnitIcon(UMaterialInterface*texture)
{

    iconWidget = CreateWidget<UUnitIconWidget>(this, iconWidgetClass);
	iconWidget->setImage(texture);
	verticalBox->AddChild(iconWidget);	
	iconArray.Emplace(iconWidget);

    

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Adding Icon!"));

}