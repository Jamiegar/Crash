// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityMapping/AbilityActionMap.h"
#include "SlateOptMacros.h"
#include "AbilityMapping/InputAbilityMappingWidget.h"
#include "Interfaces/IPluginManager.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAbilityActionMap::Construct(const FArguments& InArgs)
{
	GetImagePaths();
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight().VAlign(VAlign_Center) .HAlign(HAlign_Center)
		[
			SNew(SInputAbilityMappingWidget)
			.InputImage(InputImage_Up)
		]
		+SVerticalBox::Slot()
		.AutoHeight().VAlign(VAlign_Center) .HAlign(HAlign_Center)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
			[
				SNew(SInputAbilityMappingWidget)
				.InputImage(InputImage_Left)
			]
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
			[
				SNew(SInputAbilityMappingWidget)
				.InputImage(InputImage_Neutral)
			]
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
			[
				SNew(SInputAbilityMappingWidget)
				.InputImage(InputImage_Right)
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight().VAlign(VAlign_Center) .HAlign(HAlign_Center)
		[
			SNew(SInputAbilityMappingWidget)
			.InputImage(InputImage_Down)
		]
	];
	
}

void SAbilityActionMap::GetImagePaths()
{
	const FString Content = IPluginManager::Get().FindPlugin(TEXT("CharacterCreationTool"))->GetContentDir();

	InputImage_Down = *FString(Content + TEXT("/InputIcons/LS_Down.png"));
	InputImage_Left = *FString(Content + TEXT("/InputIcons/LS_Left.png"));
	InputImage_Right = *FString(Content + TEXT("/InputIcons/LS_Right.png"));
	InputImage_Up = *FString(Content + TEXT("/InputIcons/LS_Up.png"));
	InputImage_Neutral = *FString(Content + TEXT("/InputIcons/LS_Neutral.png"));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
