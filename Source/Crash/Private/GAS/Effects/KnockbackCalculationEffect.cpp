// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/KnockbackCalculationEffect.h"
#include "GAS/EffectCalc/KnockbackExecution.h"
#include "GAS/Effects/Knockback/KnockbackEffect.h"

UKnockbackCalculationEffect::UKnockbackCalculationEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	RemovalTagRequirements.RequireTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
	
	FGameplayEffectExecutionDefinition KnockbackExecutionDefinition;
	KnockbackExecutionDefinition.CalculationClass = UKnockbackExecution::StaticClass();
	Executions.Add(KnockbackExecutionDefinition);

	FConditionalGameplayEffect ConditionalGameplayEffect;
	ConditionalGameplayEffect.EffectClass = UKnockbackEffect::StaticClass();
	ConditionalGameplayEffects.Add(ConditionalGameplayEffect);
	
}


