// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Special/LongRangeAttackAbility.h"

#include "Characters/CrashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ULongRangeAttackAbility::ULongRangeAttackAbility()
{
	AbilityInputID = EAbilityInputID::SpecialAttackMiddle;

	KnockbackScaling = 15.0f;
	AbilityDamage = 5.0f;
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
}

void ULongRangeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility() && AttackMontage)
		return;

	OwnerCharacter = Cast<ACrashCharacter>(GetActorInfo().OwnerActor);
	if(OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
	
	FOnMontageEnded OnMontageEnded = FOnMontageEnded::CreateUObject(this, &ULongRangeAttackAbility::OnAttackMontageEnd);
	PlayAnimationMontageToOwningActor(AttackMontage, OnMontageEnded);

	WaitForDamageEffect();
}

void ULongRangeAttackAbility::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	EndCrashAbility();
}

void ULongRangeAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
	WaitForHitStopEndAndApplyKnockback(Payload);
}
