// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/DamageNotifies/AnimNotify_LaunchProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/Interfaces/CombatAbilities.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_LaunchProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(!MeshComp)
		return;

	if(ICombatAbilities* CombatAbilities = Cast<ICombatAbilities>(MeshComp->GetOwner()))
	{
		if(VFX)
		{
			const FRotator Rotation = UKismetMathLibrary::MakeRotFromX(MeshComp->GetOwner()->GetActorForwardVector());
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), VFX, MeshComp->GetBoneLocation(BoneNameFirePoint), Rotation);
		}
		
		CombatAbilities->SetProjectileSpawnTransform(MeshComp->GetSocketTransform(BoneNameFirePoint));

		if(const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
		{
			const FGameplayEventData* EventData = new FGameplayEventData();
			const FGameplayTag ProjectileFireTag = FGameplayTag::RequestGameplayTag("Event.Montage.FireProjectile");
			
			AbilitySystemInterface->GetAbilitySystemComponent()->HandleGameplayEvent(ProjectileFireTag, EventData);
		}
		
	}
}
