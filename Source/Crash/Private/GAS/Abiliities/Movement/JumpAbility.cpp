// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "AbilitySystemComponent.h"
#include "Characters/CrashCharacter.h"


UJumpAbility::UJumpAbility()
{
	AbilityInputID = MovementJump;
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.Jump")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.JumpExhausted")));
}

void UJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(ACrashCharacter* Character = Cast<ACrashCharacter>(GetActorInfo().OwnerActor))
	{
		Character->Jump();
		CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef());

		//const UAirborneEffect* Effect = UAirborneEffect::StaticClass()->GetDefaultObject<UAirborneEffect>();
		//const FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), Effect,1, 1);


		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
		
	}
}

