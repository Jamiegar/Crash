// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/Damaging/CounterAttackEffect.h"
#include "GameplayTagsManager.h"

UCounterAttackEffect::UCounterAttackEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(1.0f);
	
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Counter"));

	FGameplayEffectCue CounterCue;
	FGameplayTagContainer CounterTagContainer;
	TArray<FString> Tags;
	Tags.Add("GameplayCue.CounterAttack");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, CounterTagContainer);
	CounterCue.GameplayCueTags = CounterTagContainer;
	GameplayCues.Add(CounterCue);
}
