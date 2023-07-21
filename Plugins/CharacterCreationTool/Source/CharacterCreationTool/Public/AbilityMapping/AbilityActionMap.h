// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class CHARACTERCREATIONTOOL_API SAbilityActionMap : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAbilityActionMap)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:

	FName InputImage_Left;
	FName InputImage_Right;
	FName InputImage_Up;
	FName InputImage_Down;
	FName InputImage_Neutral;
	
	void GetImagePaths();
};
