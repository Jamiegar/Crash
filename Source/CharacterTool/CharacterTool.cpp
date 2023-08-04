// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterTool.h"
#include "BlueprintEditorModule.h"
#include "CharacterCreationToolCommands.h"
#include "CharacterCreationToolStyle.h"
#include "Menus/CharacterToolWidget.h"
#include "Modules/ModuleManager.h"


static const FName CharacterToolName("Character Creation Tool");

#define LOCTEXT_NAMESPACE "FCharacterToolModule"

void FCharacterTool::StartupModule()
{
	IModuleInterface::StartupModule();

	FCharacterCreationToolStyle::Initialize();
	FCharacterCreationToolStyle::ReloadTextures();

	FCharacterCreationToolCommands::Register();

	ToolCommands = MakeShareable(new FUICommandList);

	ToolCommands->MapAction(
		FCharacterCreationToolCommands::Get().OpenToolWindow,
		FExecuteAction::CreateRaw(this, &FCharacterTool::ToolMenuClicked),
		FCanExecuteAction());
	

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCharacterTool::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CharacterToolName, FOnSpawnTab::CreateRaw(this, &FCharacterTool::OnSpawnToolTab))
		.SetDisplayName(LOCTEXT("FCharacterToolTabTitle", "Character Creation Tool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);


	/*TSharedPtr<FExtender> Extender = MakeShareable(new FExtender);
	Extender->AddToolBarExtension("Window", EExtensionHook::After, ToolCommands, FToolBarExtensionDelegate::CreateRaw(this, &FCharacterTool::AddToolbarExtension));
	
	FBlueprintEditorModule& Module = FModuleManager::GetModuleChecked<FBlueprintEditorModule>("BlueprintEditor");
	Module.GetMenuExtensibilityManager().Get()->AddExtender(Extender);*/
}

void FCharacterTool::ShutdownModule()
{
	IModuleInterface::ShutdownModule();

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCharacterCreationToolStyle::Shutdown();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CharacterToolName);
}

void FCharacterTool::ToolMenuClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(CharacterToolName);
}

TSharedRef<SDockTab> FCharacterTool::OnSpawnToolTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCharacterTool::OnSpawnToolTab")),
		FText::FromString(TEXT("CharacterTool.cpp"))
		);

	return SNew(SDockTab)
	.TabRole(ETabRole::NomadTab)
	[
		SNew(SCharacterToolWidget)
	];
}

void FCharacterTool::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCharacterCreationToolCommands::Get().OpenToolWindow, ToolCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCharacterCreationToolCommands::Get().OpenToolWindow));
				Entry.SetCommandList(ToolCommands);
			}
		}
	}
}

void FCharacterTool::AddToolbarExtension(FToolBarBuilder& Builder)
{
	ToolMenuClicked();
}
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCharacterTool, CharacterTool);