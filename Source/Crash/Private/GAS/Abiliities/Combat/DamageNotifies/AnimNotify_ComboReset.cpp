// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/DamageNotifies/AnimNotify_ComboReset.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GAS/CrashGameplayTags.h"

void UAnimNotify_ComboReset::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(const AActor* Owner = MeshComp->GetOwner())
	{
		if(UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			const FGameplayEventData Data;
			ASC->HandleGameplayEvent(UCrashGameplayTags::GetGameplayTagFromName("Event.Montage.Combo.Reset"), &Data);
		}
	}
}
