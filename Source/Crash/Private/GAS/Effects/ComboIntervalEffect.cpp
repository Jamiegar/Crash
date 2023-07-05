// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/ComboIntervalEffect.h"

UComboIntervalEffect::UComboIntervalEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo.Count"));
}
