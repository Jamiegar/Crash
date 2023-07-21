// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/StunAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/CrashCharacter.h"
#include "GAS/Abiliities/Combat/Damage/Data/StunAbilityData.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UStunAbility::UStunAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnSpawn;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Stun"));
	
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Stun"));
}

void UStunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		return;

	const FGameplayTag StunEventTag = FGameplayTag::RequestGameplayTag("Event.Data.Stun");
	AsyncStunDataEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, StunEventTag);
	AsyncStunDataEvent->OnlyTriggerOnce = true;
	AsyncStunDataEvent->EventReceived.AddUniqueDynamic(this, &UStunAbility::OnReceivedStunData);
	AsyncStunDataEvent->Activate();
	
}

void UStunAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	OnStunDurationFinished();
}

void UStunAbility::OnReceivedStunData(FGameplayEventData Payload)
{
	TargetCharacter = CastChecked<ACrashCharacter>(Payload.Target);

	if(UStunAbilityData* StunData = Cast<UStunAbilityData>(Payload.OptionalObject))
	{
		TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);
		TargetCharacter->bIsStuned = true;


		FOnTimelineFloat TimelineFloat;
		UTimelineComponent* Timeline = TargetCharacter->GetTimelineComponent();
		Timeline->SetLooping(true);
		TimelineFloat.BindUFunction(this, FName("TimelineUpdate"));
		Timeline->AddInterpFloat(StunData->ShakeCurve, TimelineFloat);
		Timeline->PlayFromStart();
		
		AsyncStunDuration = UAbilityTask_WaitDelay::WaitDelay(this, StunData->StunDuration);
		AsyncStunDuration->OnFinish.AddUniqueDynamic(this, &UStunAbility::OnStunDurationFinished);
		AsyncStunDuration->Activate();
	}
}

void UStunAbility::OnStunDurationFinished()
{
	TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	TargetCharacter->bIsStuned = false;

	UTimelineComponent* Timeline = TargetCharacter->GetTimelineComponent();
	Timeline->Deactivate();
	AsyncStunDuration->OnFinish.Clear();
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UStunAbility::TimelineUpdate(float interpolatedValue)
{
	const FVector NewRelative = TargetCharacter->GetMesh()->GetRelativeLocation()+FVector(interpolatedValue, 0, 0);
	TargetCharacter->GetMesh()->SetRelativeLocation(NewRelative);
}
