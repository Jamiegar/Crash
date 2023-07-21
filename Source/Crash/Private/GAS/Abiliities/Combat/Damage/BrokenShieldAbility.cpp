// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/BrokenShieldAbility.h"

UBrokenShieldAbility::UBrokenShieldAbility()
{
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
}
