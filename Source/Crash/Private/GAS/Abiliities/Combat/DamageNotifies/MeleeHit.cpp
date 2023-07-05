﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/DamageNotifies/MeleeHit.h"

#include "Characters/CombatComponents/BasicCombatComponent.h"


void UMeleeHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp == nullptr)
		return;
	
	if(const AActor* OwningActor = MeshComp->GetOwner())
	{
		if(UBasicCombatComponent* Component = Cast<UBasicCombatComponent>(OwningActor->GetComponentByClass(UBasicCombatComponent::StaticClass())))
			Component->PerformDamageTrace(BoneNameTraceLocation);
	}
}