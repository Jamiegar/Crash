// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/BasicAttack.h"
#include "Characters/CrashCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"
#include "Kismet/GameplayStatics.h"

UBasicAttack::UBasicAttack()
{
	static ConstructorHelpers::FObjectFinder<UKnockbackData> DefaultKnockbackData
		(TEXT("/Script/Crash.KnockbackData'/Game/Blueprints/GAS/Abilities/Combat/Data/KnockbackData/DA_DefaultKnockback.DA_DefaultKnockback'"));

	KnockbackData = DefaultKnockbackData.Object;
	KnockbackScaling = 5;
	AbilityDamage = 2.5;
}

void UBasicAttack::PostInitProperties()
{
	Super::PostInitProperties();
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
}

void UBasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(!AttackMontage)
		return;

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;

	FOnMontageEnded Delegate = FOnMontageEnded::CreateUObject(this, &UBasicAttack::OnMontageFinished);
	PlayAnimationMontageToOwningActor(AttackMontage, Delegate);
	
	PlayMissedAttackSound();
	WaitForDamageEffect(); //Async task is created and calls OnGameplayReceivedDamageEvent when event is received
}

void UBasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if(AsyncDamageTask)
		AsyncDamageTask->EndTask();
}

void UBasicAttack::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}

void UBasicAttack::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
	WaitForHitStopEndAndApplyKnockback(Payload);

	PlayContactHitAttackSound();
}

