// Fill out your copyright notice in the Description page of Project Settings.



#include "Menus/CharacterToolWidget.h"

#include "KismetCompiler.h"
#include "SlateOptMacros.h"
#include "Characters/Input/InputAbilityMap.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "Menus/CharacterMenuBar.h"
#include "Menus/AbilityMapping/AbilityActionMap.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCharacterToolWidget::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight() .VAlign(VAlign_Top) .HAlign(HAlign_Left)
		[
			SNew(SCharacterMenuBar)
			.OnCharacterSelected(FCharacterSelected::CreateSP(this, &SCharacterToolWidget::OnCharacterSelected))
		]
		+SVerticalBox::Slot()
		.VAlign(VAlign_Center) . HAlign(HAlign_Center)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center) .Padding(10, 20)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight() .VAlign(VAlign_Top) .HAlign(HAlign_Center) .Padding(0, 10)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Basic Character Abilities"))
				]
				+SVerticalBox::Slot()
				.AutoHeight() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
				[
					SAssignNew(BasicAbilityInputMap, SAbilityActionMap)
					.OnInputSelectedAbility(FInputActionSelectedAbility::CreateSP(this, &SCharacterToolWidget::OnInputAbilitySelected))
				]
				
			]
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center) .Padding(10, 20)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight() .VAlign(VAlign_Top) .HAlign(HAlign_Center) .Padding(0, 10)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Special Character Abilities"))
				]
				+SVerticalBox::Slot()
				.AutoHeight() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
				[
					SAssignNew(SpecialAbilityInputMap, SAbilityActionMap)
					.OnInputSelectedAbility(FInputActionSelectedAbility::CreateSP(this, &SCharacterToolWidget::OnInputAbilitySelected))
				]
			]
		]
	];
	
}

void SCharacterToolWidget::OnCharacterSelected(ACrashPlayerCharacter* SelectedCharacter)
{
	CharacterSelected = SelectedCharacter;
	BasicAbilityInputMap.Get()->UpdateActionMap(CharacterSelected);
	//SpecialAbilityInputMap.Get()->UpdateActionMap(SelectedCharacter);
}

void SCharacterToolWidget::OnInputAbilitySelected(UCrashGameplayAbility* SelectedAbility, UInputAction* Action)
{
	UE_LOG(LogTemp, Warning, TEXT("Apply Ability: %s to Action %s"), *SelectedAbility->GetName(), *Action->GetName());

	if(!CharacterSelected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is not selected"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Character = %s"), *CharacterSelected->GetName());
	FAbilityInputMap& CharacterAbilityMap = CharacterSelected->GetInputAbilityMap();

	if(CharacterAbilityMap.AbilityInputMappingLayout.Contains(Action))
		CharacterAbilityMap.AbilityInputMappingLayout.Remove(Action);
	
	CharacterAbilityMap.AbilityInputMappingLayout.Add(Action, SelectedAbility->GetClass());

	BasicAbilityInputMap.Get()->UpdateActionMap(CharacterSelected);
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
