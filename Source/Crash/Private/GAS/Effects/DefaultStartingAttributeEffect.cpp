// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/DefaultStartingAttributeEffect.h"

#include "GAS/CrashAttributeSet.h"

UDefaultStartingAttributeEffect::UDefaultStartingAttributeEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;


	FGameplayModifierInfo NumberOfJumpsModifierInfo;
	NumberOfJumpsModifierInfo.Attribute = UCrashAttributeSet::GetNumberOfJumpsAttribute();
	NumberOfJumpsModifierInfo.ModifierOp = EGameplayModOp::Override;
	NumberOfJumpsModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(2);
	Modifiers.Add(NumberOfJumpsModifierInfo);

	FGameplayModifierInfo NumberOfLivesModifierInfo;
	NumberOfLivesModifierInfo.Attribute = UCrashAttributeSet::GetLivesAttribute();
	NumberOfLivesModifierInfo.ModifierOp = EGameplayModOp::Override;
	NumberOfLivesModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(3);
	Modifiers.Add(NumberOfLivesModifierInfo);

	FGameplayModifierInfo WeightModifierInfo;
	WeightModifierInfo.Attribute = UCrashAttributeSet::GetWeightAttribute();
	WeightModifierInfo.ModifierOp = EGameplayModOp::Override;
	WeightModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(16);
	Modifiers.Add(WeightModifierInfo);
}
