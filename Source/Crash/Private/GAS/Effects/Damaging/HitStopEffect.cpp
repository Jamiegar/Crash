// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/Damaging/HitStopEffect.h"
#include "GameplayTagsManager.h"

UHitStopEffect::UHitStopEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(0.5);

	FGameplayEffectCue HitStopCue;
	FGameplayTagContainer HitStopTagContainer;
	TArray<FString> Tags;
	Tags.Add("GameplayCue.HitStop");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, HitStopTagContainer);
	HitStopCue.GameplayCueTags = HitStopTagContainer;
	GameplayCues.Add(HitStopCue);
}
