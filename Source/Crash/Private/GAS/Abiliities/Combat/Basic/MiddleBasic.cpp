// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/MiddleBasic.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

UMiddleBasic::UMiddleBasic()
{
	ActivationPolicy = ECrashActivationPolicy::OnInputTriggered;
	AbilityInputID = EAbilityInputID::BasicAttackMiddle;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_MiddleAttack.AnimMon_MiddleAttack'"));

	AttackMontage = Montage.Object;

	static ConstructorHelpers::FObjectFinder<UKnockbackData> MiddleBasicKnockbackData
		(TEXT("/Script/Crash.KnockbackData'/Game/Blueprints/GAS/Abilities/Combat/Data/KnockbackData/DA_BasicMiddleKnockback.DA_BasicMiddleKnockback'"));

	KnockbackData = MiddleBasicKnockbackData.Object;
}

void UMiddleBasic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

