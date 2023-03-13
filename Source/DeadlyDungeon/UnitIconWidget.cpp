// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitIconWidget.h"

void UUnitIconWidget::setImage(UMaterialInterface* texture)
{
	IconImage->SetBrushFromMaterial(texture);
}

void UUnitIconWidget::activeTurn(bool on)
{
	if(on)
		ImageOn->SetVisibility(ESlateVisibility::Visible);
	else
		ImageOn->SetVisibility(ESlateVisibility::Hidden);
}

void UUnitIconWidget::onIconHover(bool on)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Hovered AF!"));
	
	IconHovered.Broadcast(this, on);
}