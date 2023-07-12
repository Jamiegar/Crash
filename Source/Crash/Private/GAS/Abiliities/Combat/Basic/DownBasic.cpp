// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/DownBasic.h"
#include "AbilitySystemComponent.h"

UDownBasic::UDownBasic()
{
	ActivationPolicy = ECrashActivationPolicy::OnInputTriggered;
	AbilityInputID = EAbilityInputID::BasicAttackDown;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicDownAttack.AnimMon_BasicDownAttack'"));

	AttackMontage = Montage.Object;
	AbilityDamage = 5;
	KnockbackScaling = 10.0f;
}

void UDownBasic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDownBasic::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
}
