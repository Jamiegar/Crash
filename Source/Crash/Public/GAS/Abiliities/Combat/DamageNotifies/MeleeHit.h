// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MeleeHit.generated.h"


class UNiagaraSystem;

UCLASS()
class CRASH_API UMeleeHit : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Anim Notify")
	UNiagaraSystem* ConfirmHitVFX;
	
	UPROPERTY(EditAnywhere, Category="Anim Notify")
	FName BoneNameTraceLocation;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
