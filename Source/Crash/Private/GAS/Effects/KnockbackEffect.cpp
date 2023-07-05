// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/KnockbackEffect.h"
#include "GameplayTagsManager.h"
#include "GAS/EffectCalc/KnockbackExecution.h"

UKnockbackEffect::UKnockbackEffect()
{
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.Knockback"));

	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayEffectExecutionDefinition KnockbackExecutionDefinition;
	KnockbackExecutionDefinition.CalculationClass = UKnockbackExecution::StaticClass();
	Executions.Add(KnockbackExecutionDefinition);

	FGameplayEffectCue KnockbackCue;
	FGameplayTagContainer KnockbackTagContainer;
	TArray<FString> Tags;
	Tags.Add("GameplayCue.Knockback");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, KnockbackTagContainer);
	KnockbackCue.GameplayCueTags = KnockbackTagContainer;
	GameplayCues.Add(KnockbackCue);
}


