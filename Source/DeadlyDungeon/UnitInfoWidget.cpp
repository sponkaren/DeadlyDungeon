// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitInfoWidget.h"
#include "Containers/UnrealString.h"

#include "Internationalization/Text.h"

void UUnitInfoWidget::setImage(UMaterialInterface* texture)
{
	IconImage->SetBrushFromMaterial(texture);
}

void UUnitInfoWidget::setText(FPlayerStruct playerStruct)
{
	PlayerName->SetText(FText::FromString(playerStruct.name));

	FString moveData{ MovementString + FString::FromInt(playerStruct.movement)};
	PlayerMovement->SetText(FText::FromString(moveData));
	
	FString rangeData{ RangeString + FString::FromInt(playerStruct.range) };
	PlayerRange->SetText(FText::FromString(rangeData));
	
	FString damageData{ DamageString + FString::FromInt(playerStruct.attack) };
	PlayerDamage->SetText(FText::FromString(damageData));
	
	FString attacksData{ AttacksString + FString::FromInt(playerStruct.numberOfAttacks) };
	PlayerAttacks->SetText(FText::FromString(attacksData));

}