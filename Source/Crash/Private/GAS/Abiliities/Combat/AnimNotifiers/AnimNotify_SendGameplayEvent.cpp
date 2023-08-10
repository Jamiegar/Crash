// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/AnimNotifiers/AnimNotify_SendGameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(const AActor* Owner = MeshComp->GetOwner())
	{
		if(UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			const FGameplayEventData Data;
			ASC->HandleGameplayEvent(EventTag, &Data);
		}
	}
}
