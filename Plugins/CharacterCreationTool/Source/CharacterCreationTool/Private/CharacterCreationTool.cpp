// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterCreationTool.h"
#include "CharacterCreationToolStyle.h"
#include "CharacterCreationToolCommands.h"
#include "CharacterMenuBar.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName CharacterCreationToolTabName("CharacterCreationTool");

#define LOCTEXT_NAMESPACE "FCharacterCreationToolModule"

void FCharacterCreationToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCharacterCreationToolStyle::Initialize();
	FCharacterCreationToolStyle::ReloadTextures();

	FCharacterCreationToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCharacterCreationToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FCharacterCreationToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCharacterCreationToolModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CharacterCreationToolTabName, FOnSpawnTab::CreateRaw(this, &FCharacterCreationToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FCharacterCreationToolTabTitle", "CharacterCreationTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FCharacterCreationToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCharacterCreationToolStyle::Shutdown();

	FCharacterCreationToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CharacterCreationToolTabName);
}

TSharedRef<SDockTab> FCharacterCreationToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCharacterCreationToolModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("CharacterCreationTool.cpp"))
		);

	return SNew(SDockTab)
	.TabRole(ETabRole::NomadTab)
	[
		SNew(SCharacterMenuBar)
		
	];
}

void FCharacterCreationToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(CharacterCreationToolTabName);
}

void FCharacterCreationToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCharacterCreationToolCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCharacterCreationToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCharacterCreationToolModule, CharacterCreationTool)