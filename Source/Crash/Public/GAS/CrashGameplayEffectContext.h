// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "CrashGameplayEffectContext.generated.h"


class AActor;

USTRUCT()
struct FCrashGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FCrashGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FCrashGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	static CRASH_API FCrashGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FCrashGameplayEffectContext* NewContext = new FCrashGameplayEffectContext();
		*NewContext = *this;

		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FCrashGameplayEffectContext::StaticStruct();
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
};

template<>
struct TStructOpsTypeTraits<FCrashGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FCrashGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};