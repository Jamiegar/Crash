// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/Respawn/InvincibilityEffect.h"
#include "GameplayTagsManager.h"

UInvincibilityEffect::UInvincibilityEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FGameplayEffectModifierMagnitude(5.0f);
	
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Invincible"));
	
	FGameplayEffectCue InvincibleCue;
	FGameplayTagContainer InvincibleTagContainer;
	TArray<FString> Tags;
	Tags.Add("GameplayCue.Invincible");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, InvincibleTagContainer);
	InvincibleCue.GameplayCueTags = InvincibleTagContainer;
	GameplayCues.Add(InvincibleCue);
}
