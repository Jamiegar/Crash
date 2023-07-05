// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/UpBasic.h"

UUpBasic::UUpBasic()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicUpAttack.AnimMon_BasicUpAttack'"));

	AttackMontage = Montage.Object;
}

void UUpBasic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("UpAttack"));
}
