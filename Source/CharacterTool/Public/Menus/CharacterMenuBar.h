#pragma once
#include "CoreMinimal.h"
#include "Characters/CrashPlayerCharacter.h"
#include "Widgets/SCompoundWidget.h"


DECLARE_DELEGATE_OneParam(FCharacterSelected, ACrashPlayerCharacter*)

class CHARACTERTOOL_API SCharacterMenuBar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterMenuBar)
	{}
		SLATE_EVENT(FCharacterSelected, OnCharacterSelected)

	SLATE_END_ARGS()
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OnCreateNewAbilityButtonClicked();
	FReply OnCreateCharacterButtonClicked();

	
	TSharedRef<SWidget> GenerateAssetPicker();
	void OnClassPicked(UClass* SelectedClass);
	
	FCharacterSelected OnCharacterSelected;

private:
	TSharedPtr<STextBlock> CharacterTextBox;
	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;

	
	void OnPropertyValueChanged(const FAssetData& AssetData);
};


