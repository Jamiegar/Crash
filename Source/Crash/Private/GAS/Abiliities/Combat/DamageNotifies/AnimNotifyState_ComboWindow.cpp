// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/DamageNotifies/AnimNotifyState_ComboWindow.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GAS/CrashGameplayTags.h"


void UAnimNotifyState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(const AActor* Owner = MeshComp->GetOwner())
	{
		if(UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			const FGameplayEventData Data;
			ASC->HandleGameplayEvent(UCrashGameplayTags::GetGameplayTagFromName("Event.Montage.Combo.Open"), &Data);
		}
	}
}

void UAnimNotifyState_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if(const AActor* Owner = MeshComp->GetOwner())
	{
		if(UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			const FGameplayEventData Data;
			ASC->HandleGameplayEvent(UCrashGameplayTags::GetGameplayTagFromName("Event.Montage.Combo.Close"), &Data);
		}
	}
}
