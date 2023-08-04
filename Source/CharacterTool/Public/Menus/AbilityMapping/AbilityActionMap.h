// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Widgets/SCompoundWidget.h"

class SInputAbilityMappingWidget;
class UInputAction;
class UCrashGameplayAbility;
class ACrashPlayerCharacter;

enum class EActionAbilityType
{
	Basic,
	Special
};

DECLARE_DELEGATE_TwoParams(FInputActionSelectedAbility, UCrashGameplayAbility*, UInputAction*)

class CHARACTERTOOL_API SAbilityActionMap : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAbilityActionMap)
		: _AbilityType(EActionAbilityType::Basic)
	{}
	SLATE_EVENT(FInputActionSelectedAbility, OnInputSelectedAbility)
	SLATE_ARGUMENT(EActionAbilityType, AbilityType)
	SLATE_END_ARGS()
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void UpdateActionMap(ACrashPlayerCharacter* SelectedCharacter);

private:
	TArray<TSharedPtr<SInputAbilityMappingWidget>> InputAbilityMappingWidgets;
	FInputActionSelectedAbility OnInputSelectedAbility;
	
	FName InputImage_Left;
	FName InputImage_Right;
	FName InputImage_Up;
	FName InputImage_Down;
	FName InputImage_Neutral;

	UInputAction* IA_Left = nullptr;
	UInputAction* IA_Right = nullptr;
	UInputAction* IA_Down = nullptr;
	UInputAction* IA_UP = nullptr;
	UInputAction* IA_Neutral = nullptr;
	
	void GetImagePaths();
	void SetInputActions(EActionAbilityType Type);
	
	
	UInputAction* LoadInputAction(const FString& InputActionReferencePath);

	TSharedRef<SInputAbilityMappingWidget> CreateInputAbilityMappingWidget(UInputAction* Action, FName ImagePath);
	void OnAbilitySelected(UCrashGameplayAbility* SelectedAbility, UInputAction* WidgetAction) const;
};
