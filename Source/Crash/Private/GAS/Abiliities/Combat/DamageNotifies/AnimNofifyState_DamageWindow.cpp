// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/DamageNotifies/AnimNofifyState_DamageWindow.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/CombatComponents/CombatComponent.h"
#include "Characters/Interfaces/CombatAbilities.h"
#include "Components/BoxComponent.h"
#include "GAS/Abiliities/Combat/Damage/HitBoxes/HitBox.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNofifyState_DamageWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(BoneNameSpawn.IsNone())
		return;

	SpawnLocation = MeshComp->GetBoneLocation(BoneNameSpawn);
	AbsoluteSpawnLocation = SpawnLocation;
	
	SpawnLocation = UKismetMathLibrary::InverseTransformLocation(MeshComp->GetComponentTransform(), SpawnLocation);
	SpawnLocation += HitBoxOffset;

	SpawnLocation = UKismetMathLibrary::TransformLocation(MeshComp->GetComponentTransform(), SpawnLocation);
	
	if(DamagingVFX)
	{
		const FVector RightVector = UKismetMathLibrary::GetRightVector(MeshComp->GetComponentRotation());
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), DamagingVFX, AbsoluteSpawnLocation, UKismetMathLibrary::MakeRotFromX(RightVector));
	}
	
	if(ICombatAbilities* CombatAbilities = Cast<ICombatAbilities>(MeshComp->GetOwner()))
	{
		const FVector StartLocation = MeshComp->GetOwner()->GetActorLocation();
		const FVector EndLocation = StartLocation + (MeshComp->GetOwner()->GetActorForwardVector() * 50);
		const FRotator BoxRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
		
		ActorHitBox = CombatAbilities->GetCombatComponent()->SpawnHitBox(SpawnLocation, BoxRotation);
		ActorHitBox->HitBox->SetBoxExtent(FVector(BoxExtent.Y, BoxExtent.X, BoxExtent.Z));
	}
}

void UAnimNofifyState_DamageWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(DamagingVFX && NiagaraComponent && bDestroyVFXOnNotifyEnd)
		NiagaraComponent->DestroyComponent();
	
	if(ActorHitBox)
		ActorHitBox->Destroy();
}

void UAnimNofifyState_DamageWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(MeshComp->GetWorld()->WorldType == EWorldType::EditorPreview)
		DrawDebugBox(MeshComp->GetWorld(), SpawnLocation, BoxExtent, FColor::Red);
}
