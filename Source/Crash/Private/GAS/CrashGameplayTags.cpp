// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CrashGameplayTags.h"
#include "GameplayTagsManager.h"

#pragma region StaticGameplayTagStruct

FCrashGameplayTags FCrashGameplayTags::Tags;

void FCrashGameplayTags::InitializeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	Tags.AddAllTags(GameplayTagsManager);
	GameplayTagsManager.DoneAddingNativeTags();
}

void FCrashGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Movement, "Player.Input.Move", "Left Right Movement");
	AddTag(Jump, "Player.Input.Jump", "Jump Input");
}

void FCrashGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native Tag)" + FString(TagComment))));
}
#pragma endregion

CrashGameplayTags::CrashGameplayTags()
{
}

CrashGameplayTags::~CrashGameplayTags()
{
}
