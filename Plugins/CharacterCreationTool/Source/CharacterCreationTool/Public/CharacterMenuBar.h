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
	void OnClassPicked(UClass* SelectedClass);
	void OnPropertyChanged(const FAssetData& Data);


private:
	TSharedPtr<STextBlock> CharacterTextBox;
	
	ACharacter* SelectedCharacter = nullptr;
};

