// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/ComboAttack.h"
#include "AbilitySystemComponent.h"
#include "Characters/CrashCharacter.h"
#include "GAS/Effects/ComboIntervalEffect.h"
#include "Crash/Public/GAS/Abiliities/Combat/Damage/Data/StunAbilityData.h"

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

	static ConstructorHelpers::FObjectFinder<UStunAbilityData> StunDataAsset
		(TEXT("/Script/Crash.StunAbilityData'/Game/Blueprints/GAS/Abilities/Combat/Data/StunData/DA_DefaultStunAttack.DA_DefaultStunAttack'"));

	bAttackShouldStun = true;
	StunData = StunDataAsset.Object;
}

void UComboAttack::PostInitProperties()
{
	Super::PostInitProperties();
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo"));
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Airborne"));
}

void UComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ComboMontages.IsEmpty())
		return;
	
	ComboCount = ActorInfo->AbilitySystemComponent->GetTagCount(FGameplayTag::RequestGameplayTag("Player.Combo.Count"));
	
	if(!ComboMontages.IsValidIndex(ComboCount))
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), false);
		return;
	}
	
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogTemp, Warning, TEXT("Activate Ability = %d"), ComboCount);

	if(const USkeletalMeshComponent* SkeletalMeshComponent = GetActorInfo().SkeletalMeshComponent.Get())
	{
		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();

		AnimInstance->Montage_Play(ComboMontages[ComboCount], 1);
		
		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &UComboAttack::OnMontageFinished);
		AnimInstance->Montage_SetEndDelegate(Delegate);
	}

	WaitForDamageEffect(); //Async Task waits for the damage effect
}

void UComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if(!ComboMontages.IsValidIndex(ComboCount + 1))
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo.Count"));
		BP_RemoveGameplayEffectFromOwnerWithGrantedTags(TagContainer);
	}
}

void UComboAttack::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	const ACrashCharacter* Character = CastChecked<ACrashCharacter>(GetCurrentActorInfo()->AvatarActor);
	Character->ApplyEffectToCrashCharacter(UComboIntervalEffect::StaticClass());
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}






