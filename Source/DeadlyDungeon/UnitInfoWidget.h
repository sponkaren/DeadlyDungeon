// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "PlayerCharacter.h"
#include "UnitInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEADLYDUNGEON_API UUnitInfoWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* PlayerMovement;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* PlayerDamage;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* PlayerRange;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* PlayerAttacks;
	
	UPROPERTY(BlueprintReadWrite, Category = "Character Info")
	float healthPercentage{0.5};

	const FString MovementString{"Movement: " };
	const FString DamageString{"Damage: " };
	const FString RangeString{"Range: " };
	const FString AttacksString{"Attacks: " };

public:
	bool isVisible{ true };

	void setImage(UMaterialInterface* texture);

	void setHealth(float health);

	void setText(FPlayerStruct playerStruct);

	void setVisibilty(bool on);
};
