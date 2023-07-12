// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/Input/AbilitySystemInputID.h"
#include "CrashGameplayAbility.generated.h"



UENUM(BlueprintType)
enum class ECrashActivationPolicy : uint8
{
	OnInputTriggered,
	OnSpawn
};

UCLASS(Abstract)
class CRASH_API UCrashGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCrashGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(EditCondition="ActivationPolicy == ECrashActivationPolicy::OnInputTriggered"))
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
	
	ECrashActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	
	
protected:
	bool CommitCrashAbility();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Ability Activation")
	ECrashActivationPolicy ActivationPolicy;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	FGameplayEffectSpecHandle MakeEffectSpecHandleFromAbility(const TSubclassOf<UGameplayEffect> EffectClass) const;
	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectSpecToTargetFromAbility(FGameplayEffectSpecHandle SpecHandle, FGameplayAbilityTargetDataHandle& TargetData) const;
	FActiveGameplayEffectHandle ApplyGameplayEffectSpecToOwnerFromAbility(const FGameplayEffectSpecHandle& SpecHandle) const;
};
