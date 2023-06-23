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

	FGameplayTag Movement;
	FGameplayTag Jump;
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
