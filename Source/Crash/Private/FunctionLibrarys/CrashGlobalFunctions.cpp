// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/CrashGlobalFunctions.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Characters/CrashPlayerCharacter.h"


/*TArray<TSubclassOf<ACrashPlayerCharacter>> UCrashGlobalFunctions::GetAllCharactersInContentBrowser()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FString> ContentPaths;
	ContentPaths.Add(TEXT("/Game"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	FTopLevelAssetPath BaseClassName = FTopLevelAssetPath(ACrashPlayerCharacter::StaticClass()->GetName());

	
	TSet<FTopLevelAssetPath> DerivedNames;
	TArray<FTopLevelAssetPath> BaseNames;
	BaseNames.Add(BaseClassName);

	TSet<FTopLevelAssetPath> Excluded;

	AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);

	FARFilter Filter;
	Filter.ClassPaths.Add(FTopLevelAssetPath(UBlueprint::StaticClass()->GetFName()));
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray< FAssetData > AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);
	

	for(auto const& Asset : AssetList)
	{
		// Get the the class this blueprint generates (this is stored as a full path)
		if(auto GeneratedClassPathPtr = Asset.TagsAndValues.FindTag("GeneratedClass"))
		{
			// Convert path to just the name part
			const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*GeneratedClassPathPtr);
			const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);

			// Check if this class is in the derived set
			if(!DerivedNames.Contains(*ClassName))
			{
				continue;
			}

			// Store using the path to the generated class
			Subclasses.Add(TAssetSubclassOf< UObject >(FStringAssetReference(ClassObjectPath)));
		}
	}

	
}*/
