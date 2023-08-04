// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCharacterTool : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void ToolMenuClicked();

private:

	TSharedPtr<class FUICommandList> ToolCommands;
	
	TSharedRef<class SDockTab> OnSpawnToolTab(const class FSpawnTabArgs& SpawnTabArgs);
	void RegisterMenus();
	void AddToolbarExtension(FToolBarBuilder& Builder);
	

};

