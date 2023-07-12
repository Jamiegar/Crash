﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/ComboBasic.h"

UComboBasic::UComboBasic()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LeftHookMontage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicLeftHook.AnimMon_BasicLeftHook'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RightHookMontage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicRightHook.AnimMon_BasicRightHook'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SideKickMontage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicSideKick.AnimMon_BasicSideKick'"));
	
	ComboMontages.Add(LeftHookMontage.Object);
	ComboMontages.Add(RightHookMontage.Object);
	ComboMontages.Add(SideKickMontage.Object);

	AbilityInputID = EAbilityInputID::BasicAttack;

	KnockbackScaling = 10;
}
