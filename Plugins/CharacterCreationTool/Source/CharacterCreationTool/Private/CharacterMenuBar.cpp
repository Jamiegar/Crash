// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMenuBar.h"
#include "ClassViewerModule.h"
#include "SlateOptMacros.h"
#include "Filter/FClassPickerViewFilter.h"
#include "GameFramework/Character.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCharacterMenuBar::Construct(const FArguments& InArgs)
{
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
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	TSharedRef<FClassPickerViewFilter> ClassFilter = MakeShared<FClassPickerViewFilter>();
	ClassFilter.Get().AllowedClass.Add(ACharacter::StaticClass());  

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
	SelectedCharacter = Cast<ACharacter>(SelectedClass->GetDefaultObject<ACharacter>());
	
	if(SelectedCharacter)
	{
		CharacterTextBox.Get()->SetText(FText::FromString(SelectedClass->GetName()));
		UE_LOG(LogTemp, Warning, TEXT("Selected Character = %s"), *SelectedCharacter->GetName());
	}
	
	
	FSlateApplication::Get().DismissAllMenus();
}

void SCharacterMenuBar::OnPropertyChanged(const FAssetData& Data)
{
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
