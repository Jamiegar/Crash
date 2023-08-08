// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/AirAttackAbility.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

UAirAttackAbility::UAirAttackAbility()
{
	AbilityInputID = EAbilityInputID::BasicAttackMiddle;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));
	
	SourceRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Airborne"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Grounded"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultAirAttack
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AirAttacks/AnimMon_LeftRightAirAttack.AnimMon_LeftRightAirAttack'"));

	AttackMontage = DefaultAirAttack.Object;

	static ConstructorHelpers::FObjectFinder<UKnockbackData> AirKnockbackAttack
		(TEXT("/Script/Crash.KnockbackData'/Game/Blueprints/GAS/Abilities/Combat/Data/KnockbackData/DA_BasicMiddleAir.DA_BasicMiddleAir'"));

	KnockbackData = AirKnockbackAttack.Object;
	KnockbackScaling = 15;
}

void UAirAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(CommitCrashAbility() == false)
		return;
	
	if(const USkeletalMeshComponent* SkeletalMesh = ActorInfo->SkeletalMeshComponent.Get())
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

		AnimInstance->Montage_Play(AttackMontage, 1);
		
		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &UAirAttackAbility::OnMontageFinished);
		AnimInstance->Montage_SetEndDelegate(Delegate);
	}

	WaitForDamageEffect();
}

void UAirAttackAbility::OnMontageFinished(UAnimMontage* AnimMontage, bool bInterrupted)
{
	EndCrashAbility();
}

void UAirAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);

	ApplyKnockbackInstantToTarget(Payload);
}
