// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/Knockback/KnockbackEffect.h"

#include "GameplayTagsManager.h"
#include "GAS/Abiliities/Combat/Damage/KnockBackAbility.h"

UKnockbackEffect::UKnockbackEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.Knockback"));
	
	FGameplayEffectCue KnockbackCue;
	FGameplayTagContainer KnockbackTagContainer;
	TArray<FString> Tags;
	Tags.Add("GameplayCue.ApplyedKnockback");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, KnockbackTagContainer);
	KnockbackCue.GameplayCueTags = KnockbackTagContainer;
	GameplayCues.Add(KnockbackCue);

	
	FGameplayAbilitySpecDef AbilitySpecDef;
	AbilitySpecDef.Ability = UKnockBackAbility::StaticClass();
	AbilitySpecDef.RemovalPolicy = EGameplayEffectGrantedAbilityRemovePolicy::CancelAbilityImmediately;
	
	GrantedAbilities.Add(AbilitySpecDef);
}
