// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CharacterTool : ModuleRules
{
	public CharacterTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Crash",
			"EnhancedInput",
			"EditorWidgets"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Projects",
			"EditorFramework",
			"UnrealEd",
			"ToolMenus",
			"Slate",
			"SlateCore", 
			"DesktopWidgets",
		});
		
		PublicIncludePaths.AddRange(new string[]
		{
			"CharacterTool/Public"
		});
		
		
        PrivateIncludePaths.AddRange(new string[]
        {
	        "CharacterTool/Private"
        });
		
		
	}
}
