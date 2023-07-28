// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterCreationToolCommands.h"

#define LOCTEXT_NAMESPACE "FCharacterCreationToolModule"

void FCharacterCreationToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "CharacterCreationTool", "Bring up CharacterCreationTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
