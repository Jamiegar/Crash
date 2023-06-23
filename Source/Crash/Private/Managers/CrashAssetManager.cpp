// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CrashAssetManager.h"
#include "GAS/CrashGameplayTags.h"
#include "Engine/EngineTypes.h"

UCrashAssetManager& UCrashAssetManager::Get()
{
	check(GEngine);

	UCrashAssetManager* AssetManager = Cast<UCrashAssetManager>(GEngine->AssetManager);

	return  *AssetManager;
}

void UCrashAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FCrashGameplayTags::InitializeTags();
}
