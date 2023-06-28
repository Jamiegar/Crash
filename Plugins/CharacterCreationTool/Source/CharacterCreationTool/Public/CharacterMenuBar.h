// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class CHARACTERCREATIONTOOL_API SCharacterMenuBar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterMenuBar)
		{
		}

	SLATE_END_ARGS()

	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OnCreateNewAbilityButtonClicked();
	FReply OnCreateCharacterButtonClicked();

	TSharedRef<SWidget> GenerateAssetPicker();
	void OnAssetSelected(const FAssetData& Data);
	void OnPropertyChanged(const FAssetData& Data);
};

