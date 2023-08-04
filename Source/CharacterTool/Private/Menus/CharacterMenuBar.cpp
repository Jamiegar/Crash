// Fill out your copyright notice in the Description page of Project Settings.


#include "Menus/CharacterMenuBar.h"
#include "AssetToolsModule.h"
#include "ClassViewerModule.h"
#include "PropertyCustomizationHelpers.h"
#include "SlateOptMacros.h"
#include "Characters/CrashPlayerCharacter.h"
#include "Filters/FClassPickerViewFilter.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "AssetTools/Private/AssetTools.h"
#include "Factories/CrashAbilityFactory.h"
#include "Factories/CrashCharacterFactory.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCharacterMenuBar::Construct(const FArguments& InArgs)
{
	OnCharacterSelected = InArgs._OnCharacterSelected;
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
				SAssignNew(CharacterTextBox, STextBlock)
				.Text(FText::FromString("Current Character Selected"))
			]
			.OnGetMenuContent(this, &SCharacterMenuBar::GenerateAssetPicker)
			
		]
		+SHorizontalBox::Slot()
		.AutoWidth().VAlign(VAlign_Top) .HAlign(HAlign_Center)
		[
			SNew(SButton)
			.Text(FText::FromString("Create New Ability"))
			.OnClicked(FOnClicked::CreateSP(this, &SCharacterMenuBar::OnCreateNewAbilityButtonClicked))
		]
		+SHorizontalBox::Slot()
		.AutoWidth().VAlign(VAlign_Top) .HAlign(HAlign_Right)
		[
			SNew(SButton)
			.Text(FText::FromString("Create New Character"))
			.OnClicked(FOnClicked::CreateSP(this, &SCharacterMenuBar::OnCreateCharacterButtonClicked))
		]
		/*+SHorizontalBox::Slot()
		.AutoWidth().VAlign(VAlign_Top) .HAlign(HAlign_Center)
		[
			SNew(SObjectPropertyEntryBox)
			.DisplayBrowse(true)
			.DisplayThumbnail(true)
			.EnableContentPicker(true)
			.AllowedClass(USkeletalMesh::StaticClass())
			.OnObjectChanged(FOnSetObject::CreateSP(this, &SCharacterMenuBar::OnPropertyValueChanged))
			.ThumbnailPool(AssetThumbnailPool)
			.ObjectPath("/Script/Engine.SkeletalMesh'/Game/Assets/PolygonFantasyRivals/Meshes/Characters/SK_Character_SpiritDemon.SK_Character_SpiritDemon'")
		]*/
	];
	
}

FReply SCharacterMenuBar::OnCreateNewAbilityButtonClicked()
{
	const FAssetToolsModule& ToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	
	UCrashAbilityFactory* AbilityFactory = NewObject<UCrashAbilityFactory>();
	static_cast<UAssetToolsImpl&>(ToolsModule.Get()).CreateAssetWithDialog(UCrashGameplayAbility::StaticClass(), AbilityFactory);
	
	return FReply::Handled();
}


FReply SCharacterMenuBar::OnCreateCharacterButtonClicked()
{
	const FAssetToolsModule& ToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

	UCrashCharacterFactory* CharacterFactory = NewObject<UCrashCharacterFactory>();
	static_cast<UAssetToolsImpl&>(ToolsModule.Get()).CreateAssetWithDialog(ACrashPlayerCharacter::StaticClass(), CharacterFactory);
	
	return FReply::Handled();
}

TSharedRef<SWidget> SCharacterMenuBar::GenerateAssetPicker()
{
	
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	TSharedRef<FClassPickerViewFilter> ClassFilter = MakeShared<FClassPickerViewFilter>();
	ClassFilter.Get().AllowedClass.Add(ACrashPlayerCharacter::StaticClass());  

	FClassViewerInitializationOptions Options;
	Options.ClassFilters.Add(ClassFilter);

	const TSharedRef<SWidget> ClassViewer = ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateRaw(this, &SCharacterMenuBar::OnClassPicked));
	
	return
		SNew(SBox)
		.HeightOverride(300)
		.WidthOverride(300)
		[
			ClassViewer
		];
}

void SCharacterMenuBar::OnClassPicked(UClass* SelectedClass)
{
	if(ACrashPlayerCharacter* Character = SelectedClass->GetDefaultObject<ACrashPlayerCharacter>())
	{
		CharacterTextBox.Get()->SetText(FText::FromString(SelectedClass->GetName()));
		
		OnCharacterSelected.ExecuteIfBound(Character);
	}
	
	FSlateApplication::Get().DismissAllMenus();
}


void SCharacterMenuBar::OnPropertyValueChanged(const FAssetData& AssetData)
{
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
