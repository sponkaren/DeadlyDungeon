// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UnitIconWidget.generated.h"

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

public:
	void setImage(UMaterialInterface* texture);

};
