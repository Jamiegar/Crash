// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "AbilitySystemComponent.h"
#include "Characters/CrashCharacter.h"
#include "GAS/Effects/JumpEffect.h"


UJumpAbility::UJumpAbility()
{
	AbilityInputID = MovementJump;
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.JumpExhausted")));
}

void UJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(ACrashCharacter* Character = Cast<ACrashCharacter>(GetActorInfo().OwnerActor))
	{
		if(Character->GetCrashAttributeSet()->GetNumberOfJumps() <= 0)
		{
			ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.JumpExhausted")));
			return;
		}
		
		Character->Jump();
		CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef());
		Character->ApplyEffectToCrashCharacter(UJumpEffect::StaticClass());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
		
	}
}

