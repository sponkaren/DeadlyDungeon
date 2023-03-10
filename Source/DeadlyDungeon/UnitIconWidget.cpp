// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitIconWidget.h"

void UUnitIconWidget::setImage(UMaterialInterface* texture)
{
	IconImage->SetBrushFromMaterial(texture);
}