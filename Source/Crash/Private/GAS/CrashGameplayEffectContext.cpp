// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CrashGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CrashGameplayEffectContext)


FCrashGameplayEffectContext* FCrashGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FCrashGameplayEffectContext::StaticStruct()))
	{
		return static_cast<FCrashGameplayEffectContext*>(BaseEffectContext);
	}

	return nullptr;
}

bool FCrashGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
	
	return true;
}
