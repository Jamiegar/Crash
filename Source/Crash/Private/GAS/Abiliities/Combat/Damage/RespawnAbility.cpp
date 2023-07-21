// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/RespawnAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Characters/CrashCharacter.h"
#include "Subsystems/RespawnSubsystem.h"

URespawnAbility::URespawnAbility()
{
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.Knockback"));
}

void URespawnAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("Player.State.Dead");
	AsyncWaitUntilTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, DeathTag, nullptr, true);
	AsyncWaitUntilTagRemoved->Removed.AddUniqueDynamic(this, &URespawnAbility::OnDeathTagRemoved);
	AsyncWaitUntilTagRemoved->Activate();
}

void URespawnAbility::OnDeathTagRemoved()
{
	URespawnSubsystem* RespawnSubsystem = GetWorld()->GetSubsystem<URespawnSubsystem>();
	RespawnSubsystem->StartRespawnSequence(CastChecked<ACrashCharacter>(GetActorInfo().AvatarActor));
}
