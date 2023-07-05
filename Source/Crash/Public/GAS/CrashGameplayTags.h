// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


class UGameplayTagsManager;

#pragma region StaticGameplayTagStruct
struct FCrashGameplayTags
{
public:
	static const FCrashGameplayTags& Get() { return Tags; }

	static void InitializeTags();

#pragma region MovementTags
	FGameplayTag Movement;
	FGameplayTag Jump;
#pragma endregion
#pragma region BasicCombatTags
	FGameplayTag LeftBasicAttack;
	FGameplayTag RightBasicAttack;
	FGameplayTag UpBasicAttack;
	FGameplayTag DownBasicAttack;
#pragma endregion
	FGameplayTag PlayerDamaged;
	FGameplayTag ComboCount;
	

	
protected:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:
	static FCrashGameplayTags Tags;
	
};
#pragma endregion


class CRASH_API CrashGameplayTags
{
public:
	
	CrashGameplayTags();
	~CrashGameplayTags();
};
