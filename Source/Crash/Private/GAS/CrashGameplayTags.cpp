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
	AddTag(LeftBasicAttack, "Player.Input.LeftBasicAttack", "Attack Left Basic");
	AddTag(RightBasicAttack, "Player.Input.RightBasicAttack", "Attack Right Basic");
	AddTag(UpBasicAttack, "Player.Input.UpBasicAttack", "Attack Up Basic");
	AddTag(DownBasicAttack, "Player.Input.DownBasicAttack", "Attack Down Basic");
	AddTag(BasicAttack, "Player.Input.BasicAttack", "Neutral Basic Attack");
	AddTag(Block, "Player.Input.Block", "Player presses blocking Input");
	AddTag(Slide, "Player.Input.Slide", "Player presses slide Input");
	
	AddTag(PlayerDamaged, "Player.Damaged", "Player Has Taken Damage");
}

void FCrashGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native Tag)" + FString(TagComment))));
}
#pragma endregion

UCrashGameplayTags::UCrashGameplayTags()
{
}

UCrashGameplayTags::~UCrashGameplayTags()
{
}

FGameplayTag UCrashGameplayTags::GetGameplayTagFromName(const FName Tag)
{
	return FGameplayTag::RequestGameplayTag(Tag);
}
