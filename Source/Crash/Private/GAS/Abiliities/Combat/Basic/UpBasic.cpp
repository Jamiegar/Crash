// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/UpBasic.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

UUpBasic::UUpBasic()
{
	ActivationPolicy = ECrashActivationPolicy::OnInputTriggered;
	AbilityInputID = EAbilityInputID::BasicAttackUp;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicUpAttack.AnimMon_BasicUpAttack'"));

	AttackMontage = Montage.Object;
	
	static ConstructorHelpers::FObjectFinder<UKnockbackData> UpBasicKnockbackData
		(TEXT("/Script/Crash.KnockbackData'/Game/Blueprints/GAS/Abilities/Combat/Data/KnockbackData/DA_DefaultKnockback.DA_DefaultKnockback'"));

	KnockbackData = UpBasicKnockbackData.Object;
}

void UUpBasic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
