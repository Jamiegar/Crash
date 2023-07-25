// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SClassViewer;
class UGameplayAbility;
/**
 * 
 */
class CHARACTERCREATIONTOOL_API SInputAbilityMappingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInputAbilityMappingWidget)
	{}
	SLATE_ARGUMENT(FName, InputImage)
	

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedRef<SWidget> OnMappingButtonClicked();

private:
	void OnClassPicked(UClass* SelectedClass);
};
