// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterCreationToolCommands.h"

#define LOCTEXT_NAMESPACE "FCharacterCreationToolModule"

void FCharacterCreationToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenToolWindow, "Character Creation Tool", "Bring up CharacterCreationTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
