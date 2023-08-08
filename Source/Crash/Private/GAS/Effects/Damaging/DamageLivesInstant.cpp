// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/Damaging/DamageLivesInstant.h"
#include "GAS/CrashAttributeSet.h"

UDamageLivesInstant::UDamageLivesInstant()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo LivesModifierInfo;
	LivesModifierInfo.Attribute = UCrashAttributeSet::GetLivesAttribute();
	LivesModifierInfo.ModifierOp = EGameplayModOp::Additive;
	LivesModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(-1.0f);
	Modifiers.Add(LivesModifierInfo);
}
