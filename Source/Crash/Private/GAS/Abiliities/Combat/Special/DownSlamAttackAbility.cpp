// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Special/DownSlamAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/CrashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UDownSlamAttackAbility::UDownSlamAttackAbility()
{
	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Airborne"));

	AbilityInputID = EAbilityInputID::SpecialAttackDown;	
	KnockbackScaling = 15;
	AbilityDamage = 10;
}

void UDownSlamAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		return;
	
	OwnerCharacter = CastChecked<ACrashCharacter>(GetActorInfo().OwnerActor);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);

	const FGameplayTag PauseTag = FGameplayTag::RequestGameplayTag("Event.Montage.Pause");
	AsyncEventWaitUntilPause = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PauseTag, nullptr, true, true);
	AsyncEventWaitUntilPause->EventReceived.AddUniqueDynamic(this, &UDownSlamAttackAbility::OnEventPauseMontage);
	AsyncEventWaitUntilPause->Activate();

	FOnMontageEnded OnMontageEnded = FOnMontageEnded::CreateUObject(this, &UDownSlamAttackAbility::OnAttackMontageFinished);
	PlayAnimationMontageToOwningActor(AttackMontage, OnMontageEnded);

	PlaySoundAtOwnerLocation(DownSlamSoundData.BuildUpSoundEffect);
}

void UDownSlamAttackAbility::OnEventPauseMontage(FGameplayEventData Payload)
{
	if(UAnimInstance* AnimInstance = GetActorInfo().SkeletalMeshComponent->GetAnimInstance())
	{
		AnimInstance->Montage_Pause(AttackMontage);
	}
	
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	OwnerCharacter->LaunchCharacter(LaunchDirection * LaunchForce, false, true);

	const FGameplayTag GroundedTag = FGameplayTag::RequestGameplayTag("Player.State.Grounded");
	AsyncEventWaitUntilGrounded = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GroundedTag, nullptr, true, true);
	AsyncEventWaitUntilGrounded->EventReceived.AddUniqueDynamic(this, &UDownSlamAttackAbility::OnEventGrounded);
	AsyncEventWaitUntilGrounded->Activate();
}

void UDownSlamAttackAbility::OnEventGrounded(FGameplayEventData Payload)
{
	if(UAnimInstance* AnimInstance = GetActorInfo().SkeletalMeshComponent->GetAnimInstance())
	{
		AnimInstance->Montage_Resume(AttackMontage);
	}

	PlaySoundAtOwnerLocation(DownSlamSoundData.GroundSlamSoundEffect);
	WaitForDamageEffect();
}

void UDownSlamAttackAbility::OnAttackMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	EndCrashAbility();
}

void UDownSlamAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
	WaitForHitStopEndAndApplyKnockback(Payload);
}
