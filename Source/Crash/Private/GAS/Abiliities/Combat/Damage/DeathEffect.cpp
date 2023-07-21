// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/DeathEffect.h"

#include "GameplayTagsManager.h"

UDeathEffect::UDeathEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Dead"));
	InheritableBlockedAbilityTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.Knockback"));
	InheritableBlockedAbilityTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Invincible"));
	
	const FGameplayEffectModifierMagnitude ModifierMagnitude = FGameplayEffectModifierMagnitude(1.2);
	DurationMagnitude = ModifierMagnitude;

	FGameplayEffectCue DeathCue;
	FGameplayTagContainer DeathTagContainer;
	TArray<FString> Tags;
	Tags.Add("GameplayCue.Death");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, DeathTagContainer);
	DeathCue.GameplayCueTags = DeathTagContainer;
	GameplayCues.Add(DeathCue);

	
}
