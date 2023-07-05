// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/DamageBasicInstant.h"

#include "GameplayTagsManager.h"
#include "GAS/Abiliities/Combat/Damage/StunAbility.h"
#include "GAS/EffectCalc/DamageExecution.h"
#include "GAS/Effects/KnockbackEffect.h"

UDamageBasicInstant::UDamageBasicInstant()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayEffectExecutionDefinition DamageExecutionDefinition;
	DamageExecutionDefinition.CalculationClass = UDamageExecution::StaticClass();
	
	Executions.Add(DamageExecutionDefinition);
	
	FGameplayEffectCue DamageCue;
	FGameplayTagContainer DamageTagContainer;
	TArray<FString> Tags;
	Tags.Add("GameplayCue.TakeDamage");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, DamageTagContainer);
	DamageCue.GameplayCueTags = DamageTagContainer;
	GameplayCues.Add(DamageCue);

	FGameplayAbilitySpecDef AbilitySpecDef;
	AbilitySpecDef.Ability = UStunAbility::StaticClass();
	AbilitySpecDef.RemovalPolicy = EGameplayEffectGrantedAbilityRemovePolicy::RemoveAbilityOnEnd;
	GrantedAbilities.Add(AbilitySpecDef);
}
