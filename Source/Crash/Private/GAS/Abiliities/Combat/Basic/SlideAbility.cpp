// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/SlideAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Crash/Public/GAS/Abiliities/Combat/Damage/Data/StunAbilityData.h"
#include "GAS/Abiliities/Combat/Damage/BrokenShieldAbility.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

USlideAbility::USlideAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnInputTriggered;
	AbilityInputID = EAbilityInputID::Slide;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultSlideMon
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_SlideAttack.AnimMon_SlideAttack'"));

	SlideMontage = DefaultSlideMon.Object;

	static ConstructorHelpers::FObjectFinder<UStunAbilityData> DefaultStunData
		(TEXT("/Script/Crash.StunAbilityData'/Game/Blueprints/GAS/Abilities/Combat/Data/StunData/DA_DefaultStunAttack.DA_DefaultStunAttack'"));
	
	bAttackShouldStun = true;
	StunData = DefaultStunData.Object;

	static ConstructorHelpers::FObjectFinder<UKnockbackData> SlideKnockbackData
		(TEXT("/Script/Crash.KnockbackData'/Game/Blueprints/GAS/Abilities/Combat/Data/KnockbackData/DA_SlideKnockback.DA_SlideKnockback'"));

	KnockbackData = SlideKnockbackData.Object;
	
}

void USlideAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;
	
	if(const USkeletalMeshComponent* SkeletalMesh = ActorInfo->SkeletalMeshComponent.Get())
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

		AnimInstance->Montage_Play(SlideMontage, 1);
		
		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &USlideAbility::OnMontageFinished);
		AnimInstance->Montage_SetEndDelegate(Delegate);
	}

	WaitForDamageEffect();
}

void USlideAbility::OnMontageFinished(UAnimMontage* AnimMontage, bool bInterrupted)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}

void USlideAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
	
	UAbilitySystemComponent* TargetComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Payload.Target);
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(UBrokenShieldAbility::StaticClass());
	TargetComponent->GiveAbilityAndActivateOnce(AbilitySpec);
	
	ApplyKnockbackInstantToTarget(Payload);
}
