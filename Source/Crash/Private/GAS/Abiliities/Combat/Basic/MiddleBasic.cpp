// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/MiddleBasic.h"

UMiddleBasic::UMiddleBasic()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicLeftHook.AnimMon_BasicLeftHook'"));

	AttackMontage = Montage.Object;
}

void UMiddleBasic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

