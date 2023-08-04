// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CharacterCreationToolStyle.h"

class FCharacterCreationToolCommands : public TCommands<FCharacterCreationToolCommands>
{
public:

	FCharacterCreationToolCommands()
		: TCommands<FCharacterCreationToolCommands>(TEXT("CharacterCreationTool"), NSLOCTEXT("Contexts", "CharacterCreationTool", "CharacterCreationTool Plugin"), NAME_None, FCharacterCreationToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenToolWindow;
};