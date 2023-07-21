// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/BasicAttack.h"
#include "Characters/CrashCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

UBasicAttack::UBasicAttack()
{
	static ConstructorHelpers::FObjectFinder<UKnockbackData> DefaultKnockbackData
		(TEXT("/Script/Crash.KnockbackData'/Game/Blueprints/GAS/Abilities/Combat/Data/KnockbackData/DA_DefaultKnockback.DA_DefaultKnockback'"));

	KnockbackData = DefaultKnockbackData.Object;
	KnockbackScaling = 15;
}

void UBasicAttack::PostInitProperties()
{
	Super::PostInitProperties();
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));

	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
	
}

void UBasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(!AttackMontage)
		return;

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;
	
	if(const USkeletalMeshComponent* SkeletalMesh = ActorInfo->SkeletalMeshComponent.Get())
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

		AnimInstance->Montage_Play(AttackMontage, 1);
		
		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &UBasicAttack::OnMontageFinished);
		AnimInstance->Montage_SetEndDelegate(Delegate);
	}

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
	ApplyKnockbackToTarget(Payload);
}

