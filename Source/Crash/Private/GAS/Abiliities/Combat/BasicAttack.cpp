// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/BasicAttack.h"
#include "Characters/CrashCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UBasicAttack::UBasicAttack()
{
}

void UBasicAttack::PostInitProperties()
{
	Super::PostInitProperties();
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
}

void UBasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(!AttackMontage)
		return;

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
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

	if(AsyncTask)
		AsyncTask->EndTask();
}

void UBasicAttack::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}

void UBasicAttack::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Receaved Payload"));
	Super::OnGameplayReceivedDamageEvent(Payload);
}

