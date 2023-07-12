// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/SlideAbility.h"

USlideAbility::USlideAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnInputTriggered;
	AbilityInputID = EAbilityInputID::Slide;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultSlideMon
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_SlideAttack.AnimMon_SlideAttack'"));

	SlideMontage = DefaultSlideMon.Object;
}

void USlideAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(const USkeletalMeshComponent* SkeletalMesh = ActorInfo->SkeletalMeshComponent.Get())
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

		AnimInstance->Montage_Play(SlideMontage, 1);
		
		FOnMontageEnded Delegate;
		/*Delegate.BindUObject(this, &UBasicAttack::OnMontageFinished);
		AnimInstance->Montage_SetEndDelegate(Delegate);*/

		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
	}
}
