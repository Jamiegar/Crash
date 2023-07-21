﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/KnockBackAbility.h"

#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Characters/CrashCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/CombatComponents/KnockbackComponent.h"
#include "GAS/CrashAttributeSet.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

UKnockBackAbility::UKnockBackAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnSpawn;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.Knockback"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Stun"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultGetUp
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/GetUp/AnimMon_GetUp.AnimMon_GetUp'"));
	
	GetUpMontage = DefaultGetUp.Object;
}

void UKnockBackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
	

	FGameplayTag DataTag = FGameplayTag::RequestGameplayTag("Event.Data.Knockback");
	AsyncEventKnockbackData = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DataTag);
	AsyncEventKnockbackData->EventReceived.AddUniqueDynamic(this, &UKnockBackAbility::OnReceivedKnockbackData);
	AsyncEventKnockbackData->Activate();
}

void UKnockBackAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	FGameplayTagContainer KnockbackTagContainer;
	TArray<FString> Tags;
	Tags.Add("Player.Damaged.Knockback");
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, KnockbackTagContainer);
	
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(KnockbackTagContainer);
}

void UKnockBackAbility::OnReceivedKnockbackData(FGameplayEventData Payload)
{
	KnockbackData = Cast<UKnockbackData>(Payload.OptionalObject);

	if(!KnockbackData)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
		return;
	}
	
	FVector Direction = FVector(Payload.Target->GetActorLocation() - Payload.Instigator->GetActorLocation());
	Direction.Normalize();
	Direction += FVector(0, 0, 1);
	
	UE_LOG(LogTemp, Warning, TEXT("Direction: %s"), *Direction.ToString());
	TargetCharacter = CastChecked<ACrashCharacter>(Payload.Target);
	
	if(const UCrashAttributeSet* Attribute = TargetCharacter->GetCrashAttributeSet())
	{
		float KnockbackMag = Attribute->GetKnockback();

		FVector LaunchVelocity = Direction * KnockbackMag;
		LaunchVelocity.X *= KnockbackData->HorizontalForce;
		LaunchVelocity.Z *= KnockbackData->UpwardForce;
		
		TargetCharacter->LaunchCharacter(LaunchVelocity, true, true);
		TargetCharacter->bIsKnockedBack = true;

		const FGameplayTag GroundedTag = FGameplayTag::RequestGameplayTag("Player.State.Grounded");
		AsyncEventWaitUntilGrounded = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GroundedTag, nullptr, true, true);
		AsyncEventWaitUntilGrounded->EventReceived.AddUniqueDynamic(this, &UKnockBackAbility::OnCharacterGrounded);
		AsyncEventWaitUntilGrounded->Activate();
	}
	
}


void UKnockBackAbility::OnCharacterGrounded(FGameplayEventData Payload)
{
	WaitAsyncKncokbackEnd = UAbilityTask_WaitDelay::WaitDelay(this, KnockbackData->KnockbackGroundedDuration);
	WaitAsyncKncokbackEnd->OnFinish.AddUniqueDynamic(this, &UKnockBackAbility::OnKnockbackGroundedFinished);
	WaitAsyncKncokbackEnd->Activate();

	
	TargetCharacter->GetKnockbackComponent()->StopScrew();
}

void UKnockBackAbility::OnKnockbackGroundedFinished()
{
	if(const USkeletalMeshComponent* SkeletalMeshComponent = GetActorInfo().SkeletalMeshComponent.Get())
	{
		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

		AnimInstance->Montage_Play(GetUpMontage, 1);
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);

	FGameplayTagContainer KnockbackTagContainer;
	TArray<FString> Tags;
	Tags.Add("Player.Damaged.Knockback");
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, KnockbackTagContainer);
	
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(KnockbackTagContainer);
}
