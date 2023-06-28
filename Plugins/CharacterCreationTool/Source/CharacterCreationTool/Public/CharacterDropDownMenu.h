// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class CHARACTERCREATIONTOOL_API SCharacterDropDownMenu : public SObjectPropertyEntryBox
{
public:
	SLATE_BEGIN_ARGS(SCharacterDropDownMenu)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
