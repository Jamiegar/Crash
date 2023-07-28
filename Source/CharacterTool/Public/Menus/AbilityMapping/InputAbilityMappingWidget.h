// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


class UInputAction;
class UCrashGameplayAbility;
class ACrashPlayerCharacter;
class SClassViewer;
class UGameplayAbility;


DECLARE_DELEGATE_TwoParams(FSelectedAbility, UCrashGameplayAbility*, UInputAction*)

class CHARACTERTOOL_API SInputAbilityMappingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInputAbilityMappingWidget)
	{}
	
	SLATE_ARGUMENT(FName, InputImage)
	SLATE_ARGUMENT(UInputAction*, Action)
	SLATE_EVENT(FSelectedAbility, OnAbilitySelected)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	TSharedRef<SWidget> OnMappingButtonClicked();

private:
	UInputAction* WidgetInputActionType = nullptr;
	FSelectedAbility OnAbilitySelected;
	
	void OnClassPicked(UClass* SelectedClass);
};
