// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/ComboAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Crash/Public/GAS/Abiliities/Combat/Damage/Data/StunAbilityData.h"
#include "GAS/CrashGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

UComboAttack::UComboAttack()
{
	static ConstructorHelpers::FObjectFinder<UStunAbilityData> StunDataAsset
		(TEXT("/Script/Crash.StunAbilityData'/Game/Blueprints/GAS/Abilities/Combat/Data/StunData/DA_DefaultStunAttack.DA_DefaultStunAttack'"));

	bAttackShouldStun = true;
	StunData = StunDataAsset.Object;

	static ConstructorHelpers::FObjectFinder<UKnockbackData> DefaultKnockbackData
		(TEXT("/Script/Crash.KnockbackData'/Game/Blueprints/GAS/Abilities/Combat/Data/KnockbackData/DA_DefaultKnockback.DA_DefaultKnockback'"));
	
	KnockbackData = DefaultKnockbackData.Object;
}

void UComboAttack::PostInitProperties()
{
	Super::PostInitProperties();
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Airborne"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
}

void UComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ComboMontages.IsEmpty())
		return;
	
	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;
	
	ComboCount = 0;

	NextAttackInSequence();
}

void UComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}

void UComboAttack::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);

	if(ComboCount == ComboMontages.Num()-1)
	{
		ApplyKnockbackToTarget(Payload);
	}
	else
	{
		ApplyStunToTarget(Payload);
	}
}

void UComboAttack::NextAttackInSequence()
{
	MontageWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "None", GetMontageToPlay());

	MontageWaitTask->OnCompleted.AddUniqueDynamic(this, &UComboAttack::OnMontageFinished);
	MontageWaitTask->Activate();

	bIsComboWindowOpen = false;
	WaitForDamageEffect(); //Async Task waits for the damage effect

	const FGameplayTag ComboOpenTag = UCrashGameplayTags::GetGameplayTagFromName("Event.Montage.Combo.Open");
	AsyncComboOpenEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboOpenTag, nullptr, true);
	AsyncComboOpenEvent->EventReceived.AddUniqueDynamic(this, &UComboAttack::OnGameplayReceivedOnComboOpen);
	AsyncComboOpenEvent->Activate();
	
	const FGameplayTag ComboCloseTag = UCrashGameplayTags::GetGameplayTagFromName("Event.Montage.Combo.Close");
	AsyncComboCloseEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboCloseTag, nullptr, true);
	AsyncComboCloseEvent->EventReceived.AddUniqueDynamic(this, &UComboAttack::OnGameplayReceivedComboClose);
	AsyncComboCloseEvent->Activate();

	const FGameplayTag ComboRestTag =UCrashGameplayTags::GetGameplayTagFromName("Event.Montage.Combo.Reset");
	AsyncComboResetEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboRestTag, nullptr, true);
	AsyncComboResetEvent->EventReceived.AddUniqueDynamic(this, &UComboAttack::OnGameplayReceivedComboRest);
	AsyncComboResetEvent->Activate();
}

UAnimMontage* UComboAttack::GetMontageToPlay()
{
	if(ComboMontages.IsValidIndex(ComboCount))
		return ComboMontages[ComboCount];

	return nullptr;
}

void UComboAttack::OnMontageFinished()
{
	if(!bDidRequestCombo)	
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}

void UComboAttack::OnGameplayReceivedOnComboOpen(FGameplayEventData Payload)
{
	bDidRequestCombo = false;
	bIsComboWindowOpen = true;

	if(DoesNextAttackExist())
	{
		AsyncComboInputPressed = UAbilityTask_WaitInputPress::WaitInputPress(this);
		AsyncComboInputPressed->OnPress.AddUniqueDynamic(this, &UComboAttack::OnAbilityInputPressed);
		AsyncComboInputPressed->Activate();
	}
}

void UComboAttack::OnGameplayReceivedComboClose(FGameplayEventData Payload)
{
	bIsComboWindowOpen = false;

	if(bDidRequestCombo)
	{
		ComboCount++;
		NextAttackInSequence();
	}
}

void UComboAttack::OnGameplayReceivedComboRest(FGameplayEventData Payload)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}

void UComboAttack::OnAbilityInputPressed(float TimeWaited)
{
	if(bIsComboWindowOpen)
		bDidRequestCombo = true;
}






