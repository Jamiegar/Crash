// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "AbilitySystemComponent.h"
#include "Characters/CrashCharacter.h"
#include "GAS/CrashAttributeSet.h"
#include "GAS/Effects/JumpEffect.h"
#include "GAS/Effects/JumpExhaustedEffect.h"


UJumpAbility::UJumpAbility()
{
	AbilityInputID = EAbilityInputID::MovementJump;
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.JumpExhausted"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Cooldown.Jump"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.Knockback"));
}



void UJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(ACrashCharacter* Character = CastChecked<ACrashCharacter>(GetActorInfo().AvatarActor))
	{
		CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef());

		Character->Jump();
		Character->ApplyEffectToCrashCharacter(UJumpEffect::StaticClass());
		
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
		const UCrashAttributeSet* Attributes = Character->GetCrashAttributeSet();
		
		if(Attributes)
		{
			if(Attributes->GetNumberOfJumps() <= 0)
				Character->ApplyEffectToCrashCharacter(UJumpExhaustedEffect::StaticClass());
		}
		
	}
}

