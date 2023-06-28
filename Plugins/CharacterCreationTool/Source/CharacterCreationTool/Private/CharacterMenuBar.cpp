// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMenuBar.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "PropertyCustomizationHelpers.h"
#include "SlateOptMacros.h"
#include "GameFramework/Character.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SCharacterMenuBar::Construct(const FArguments& InArgs)
{
	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth() .VAlign(VAlign_Top) .HAlign(HAlign_Left)
		[
			SNew(SComboButton)
			.ButtonContent()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Current Character Selected"))
			]
			.OnGetMenuContent(this, &SCharacterMenuBar::GenerateAssetPicker)
			
			
			/*SNew(SObjectPropertyEntryBox)
			.OnObjectChanged(this, &SCharacterMenuBar::OnPropertyChanged)
			.AllowedClass(UStaticMesh::StaticClass())
			.DisplayThumbnail(true)
			.DisplayBrowse(true)
			.EnableContentPicker(true)
			.DisplayCompactSize(false)
			.AllowClear(true)
			.PropertyHandle()*/
			

		]
		+SHorizontalBox::Slot()
		.AutoWidth().VAlign(VAlign_Top) .HAlign(HAlign_Left)
		[
			SNew(SButton)
			.Text(FText::FromString("Create New Ability"))
			.OnClicked(FOnClicked::CreateSP(this, &SCharacterMenuBar::OnCreateNewAbilityButtonClicked))
		]
		+SHorizontalBox::Slot()
		.AutoWidth().VAlign(VAlign_Top) .HAlign(HAlign_Left)
		[
			SNew(SButton)
			.Text(FText::FromString("Create New Character"))
			.OnClicked(FOnClicked::CreateSP(this, &SCharacterMenuBar::OnCreateCharacterButtonClicked))
		]
	];
	
}

FReply SCharacterMenuBar::OnCreateNewAbilityButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Create New Ability"));
	return FReply::Handled();
}

FReply SCharacterMenuBar::OnCreateCharacterButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Create New Charater"));
	return FReply::Handled();
}

TSharedRef<SWidget> SCharacterMenuBar::GenerateAssetPicker()
{
	const UClass* AllowedClass = Cast<UClass>(UBlueprint::StaticClass());
	const FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	UE_LOG(LogTemp, Warning, TEXT("Path Name: %s"), *AllowedClass->GetClassPathName().GetAssetName().ToString());
	
	FAssetPickerConfig PickerConfig;
	PickerConfig.Filter.ClassPaths.Add(AllowedClass->GetClassPathName());
	PickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SCharacterMenuBar::OnAssetSelected);
	PickerConfig.bAllowNullSelection = true;
	PickerConfig.Filter.bRecursiveClasses = true;
	PickerConfig.InitialAssetViewType = EAssetViewType::List;
	PickerConfig.bAllowDragging = false;
	PickerConfig.bCanShowFolders = true;

	
	
	return
		SNew(SBox)
		.HeightOverride(300)
		.WidthOverride(300)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Menu.Background"))
			[
				ContentBrowserModule.Get().CreateAssetPicker(PickerConfig)
			]
		];

}

void SCharacterMenuBar::OnAssetSelected(const FAssetData& Data)
{
}

void SCharacterMenuBar::OnPropertyChanged(const FAssetData& Data)
{
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
