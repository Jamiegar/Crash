// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/ComboAttack.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "GAS/CrashGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UComboAttack::UComboAttack()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LeftHookMontage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicLeftHook.AnimMon_BasicLeftHook'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RightHookMontage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicRightHook.AnimMon_BasicRightHook'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SideKickMontage
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/BasicCombat/AnimMon_BasicSideKick.AnimMon_BasicSideKick'"));
	
	ComboMontages.Add(LeftHookMontage.Object);
	ComboMontages.Add(RightHookMontage.Object);
	ComboMontages.Add(SideKickMontage.Object);
}

void UComboAttack::PostInitProperties()
{
	Super::PostInitProperties();

	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
	ActivationOwnedTags.AddTag(GameTags.ComboCount);

	//ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Attack")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
	
}

void UComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ComboMontages.IsEmpty())
		return;
	
	
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	ComboCount = ActorInfo->AbilitySystemComponent->GetTagCount(GameTags.ComboCount);
	
	UE_LOG(LogTemp, Warning, TEXT("Activate Ability = %d"), ComboCount -1);
	

	if(const USkeletalMeshComponent* SkeletalMeshComponent = GetActorInfo().SkeletalMeshComponent.Get())
	{
		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

		if(ComboMontages.IsValidIndex(ComboCount-1))
			AnimInstance->Montage_Play(ComboMontages[ComboCount-1], 1);
		else
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, true);
		
		

		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &UComboAttack::OnMontageFinished);
		AnimInstance->Montage_SetEndDelegate(Delegate);
	}

	WaitForDamageEffect(); //Async Task waits for the damage effect
	
	if(WaitDelayAsync == nullptr)
	{
		WaitDelayAsync = UAbilityTask_WaitDelay::WaitDelay(this, 1.5f);
		WaitDelayAsync->OnFinish.AddUniqueDynamic(this, &UComboAttack::OnWaitFinished);
		WaitDelayAsync->Activate();
	}
}

void UComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	WaitDelayAsync->OnFinish.RemoveDynamic(this, &UComboAttack::OnWaitFinished);
	
	if(WaitDelayAsync != nullptr)
		if(WaitDelayAsync->IsActive())
			WaitDelayAsync->EndTask();

	FGameplayTagContainer TagContainer;
	TArray<FString> Tags;
	Tags.Add("Player.Combo.Count");
	Tags.Add("Player.Attack");
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, TagContainer);
	
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(TagContainer, ComboCount);
}

void UComboAttack::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if(ComboCount >= ComboMontages.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Should End Ability"));
		WaitDelayAsync->EndTask();
	}
}

void UComboAttack::OnWaitFinished()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
	UE_LOG(LogTemp, Warning, TEXT("End Ability Combo"));
}

