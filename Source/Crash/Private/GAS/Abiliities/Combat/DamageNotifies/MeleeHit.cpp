// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/DamageNotifies/MeleeHit.h"
#include "Characters/CombatComponents/CombatComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"


void UMeleeHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp == nullptr)
		return;
	
	if(const AActor* OwningActor = MeshComp->GetOwner())
	{
		if(UCombatComponent* Component = Cast<UCombatComponent>(OwningActor->GetComponentByClass(UCombatComponent::StaticClass())))
		{
			if(Component->PerformDamageTrace(BoneNameTraceLocation) && ConfirmHitVFX != nullptr)
			{
				const FVector SpawnLocation = MeshComp->GetBoneLocation(BoneNameTraceLocation);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetOwner(), ConfirmHitVFX, SpawnLocation);
			}
		}
		
	}
}
