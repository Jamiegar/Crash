// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SAbilityActionMap;
class UInputAction;
class UCrashGameplayAbility;
class ACrashPlayerCharacter;

class CHARACTERTOOL_API SCharacterToolWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterToolWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	ACrashPlayerCharacter* CharacterSelected = nullptr;
	TSharedPtr<SAbilityActionMap> BasicAbilityInputMap = nullptr;
	TSharedPtr<SAbilityActionMap> SpecialAbilityInputMap = nullptr;
	
	void OnCharacterSelected(ACrashPlayerCharacter* SelectedCharacter);
	void OnInputAbilitySelected(UCrashGameplayAbility* SelectedAbility, UInputAction* Action);
};
