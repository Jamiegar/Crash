// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNofifyState_DamageWindow.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class AHitBox;
/**
 * 
 */
UCLASS()
class CRASH_API UAnimNofifyState_DamageWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* DamagingVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=VFX)
	bool bDestroyVFXOnNotifyEnd = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitBox")
	FName BoneNameSpawn = FName(NAME_None);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitBox")
	FVector HitBoxOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitBox")
	FVector BoxExtent = FVector(32, 32, 32);
	
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	AHitBox* ActorHitBox;

	UPROPERTY()
	FVector SpawnLocation;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent;
	FVector AbsoluteSpawnLocation; 
};
