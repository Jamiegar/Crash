// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/KnockBackAbility.h"
#include "Characters/CrashCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/CrashAttributeSet.h"

UKnockBackAbility::UKnockBackAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnSpawn;
	
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

void UKnockBackAbility::OnReceivedKnockbackData(FGameplayEventData Payload)
{
	FVector Direction = FVector(Payload.Target->GetActorLocation() - Payload.Instigator->GetActorLocation());
	Direction.Normalize();
	Direction += FVector(0,0,1);
	
	UE_LOG(LogTemp, Warning, TEXT("Direction: %s"), *Direction.ToString());
	ACrashCharacter* TargetCharacter = CastChecked<ACrashCharacter>(Payload.Target);
	
	if(UCrashAttributeSet* Attribute = TargetCharacter->GetCrashAttributeSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute is valid"));
		float KnockbackMag = Attribute->GetKnockback();
		TargetCharacter->LaunchCharacter(Direction * KnockbackMag, true, true);
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}
