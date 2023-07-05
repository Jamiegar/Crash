// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/DownBasic.h"
#include "GAS/CrashGameplayTags.h"
#include "GAS/Effects/KnockbackEffect.h"

UDownBasic::UDownBasic()
{
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

	const FGameplayEffectSpecHandle Handle = MakeEffectSpecHandleFromAbility(UKnockbackEffect::StaticClass());

	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	FGameplayEffectSpec* Spec = Handle.Data.Get();
	Spec->SetSetByCallerMagnitude("Player.Damaged.Knockback", KnockbackScaling);

	ApplyAbilityTagsToGameplayEffectSpec(*Handle.Data.Get(), GetCurrentAbilitySpec());
	
	auto ActiveGameplayEffectHandles= ApplyGameplayEffectSpecToTargetFromAbility(Handle, Payload.TargetData);
}
