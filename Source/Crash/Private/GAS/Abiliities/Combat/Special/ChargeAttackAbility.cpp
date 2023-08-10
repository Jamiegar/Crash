// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Special/ChargeAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Characters/CrashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UChargeAttackAbility::UChargeAttackAbility()
{
	AbilityInputID = EAbilityInputID::SpecialAttackMiddle;

	AbilityDamage = 8;
	KnockbackScaling = 15;
}

void UChargeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AsyncWaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	AsyncWaitInputRelease->OnRelease.AddUniqueDynamic(this, &UChargeAttackAbility::OnAbilityInputRelease);
	AsyncWaitInputRelease->Activate();

	FOnMontageEnded AttackMontageEnded = FOnMontageEnded::CreateUObject(this, &UChargeAttackAbility::OnAttackMontageFinished);
	PlayAnimationMontageToOwningActor(AttackMontage, AttackMontageEnded);

	const FGameplayTag MontagePausedTag = FGameplayTag::RequestGameplayTag("Event.Montage.Pause");
	AsyncWaitMontagePaused = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, MontagePausedTag);
	AsyncWaitMontagePaused->EventReceived.AddUniqueDynamic(this, &UChargeAttackAbility::OnAttackMontagePaused);
	AsyncWaitMontagePaused->Activate();

	OwnerCharacter = CastChecked<ACrashCharacter>(GetActorInfo().OwnerActor);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);

	GetWorld()->GetTimerManager().SetTimer(AbilityMaxDurationTimer, this, &UChargeAttackAbility::OnMaxAbilityDuration, MaxInputDuration, false);
}

void UChargeAttackAbility::OnAttackMontagePaused(FGameplayEventData Payload)
{
	if(bAbilityInputRelease)
		return;
	
	if(UAnimInstance* AnimInstance = GetActorInfo().SkeletalMeshComponent->GetAnimInstance())
	{
		AnimInstance->Montage_Pause(AttackMontage);
	}
}

void UChargeAttackAbility::OnAbilityInputRelease(float Duration)
{
	bAbilityInputRelease = true;
	
	if(AsyncWaitMontagePaused->IsActive())
		AsyncWaitMontagePaused->EndTask();
	
	if(Duration > MaxInputChargeDuration)
		Duration = MaxInputChargeDuration;
	
	AbilityDamage = ConstantBaseAttackDamage + ((Duration / MaxInputChargeDuration) * DamageMultiplier);

	if(UAnimInstance* AnimInstance = GetActorInfo().SkeletalMeshComponent->GetAnimInstance())
	{
		AnimInstance->Montage_Resume(AttackMontage);
	}
	GetWorld()->GetTimerManager().ClearTimer(AbilityMaxDurationTimer);
	
	WaitForDamageEffect();
}

void UChargeAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
	WaitForHitStopEndAndApplyKnockback(Payload);
}

void UChargeAttackAbility::OnMaxAbilityDuration()
{
	OnAbilityInputRelease(MaxInputChargeDuration);
}

void UChargeAttackAbility::OnAttackMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	EndCrashAbility();
}
