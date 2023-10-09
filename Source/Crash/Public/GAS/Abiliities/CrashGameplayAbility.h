// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/Input/AbilitySystemInputID.h"
#include "Animation/AnimMontage.h"
#include "CrashGameplayAbility.generated.h"



UENUM(BlueprintType)
enum class ECrashActivationPolicy : uint8
{
	OnInputTriggered,
	OnSpawn
};

/**********************************************************************************
* Each ability inherits from this abstract Gameplay Ability
***********************************************************************************/
UCLASS(Abstract, Blueprintable)
class CRASH_API UCrashGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/* Constructor is used to set up ability (Adding gameplay tags) */
	UCrashGameplayAbility();

	/**********************************************************************************
	* Ability Activation
	***********************************************************************************/

	/*
	 * NOTE	!! 
	 * If the activation policy is set to 'OnInputTrigger' this enum sets the input type that will trigger the ability.
	 * The 'AbilityInputID' byte is then used by GAS to trigger the correct ability on the character 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(EditCondition="ActivationPolicy == ECrashActivationPolicy::OnInputTriggered"))
	EAbilityInputID AbilityInputID = EAbilityInputID::None;

	/* Returns the 'ActivationPolicy' type */
	ECrashActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	/* Trys to activate the ability when spawned if 'ActivationPolicy' is set to 'OnSpawn' */
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:

	/*
	 * NOTE !!
	 * Activation Policy can be changed in editor but can only be read from Blueprint.
	 * 
	 * The 'ActivationPolicy' determines how the ability is triggered / activated
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Ability Activation")
	ECrashActivationPolicy ActivationPolicy;

	/* Only for c++ use to commit an ability using the default method */
	bool CommitCrashAbility();

	/* Only for c++ use to End an ability using the default method */
	void EndCrashAbility();

	/* Triggered when ability is given to to a 'GameplayAbilityComponent' */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/* Plays animation montage on the owning actor of this ability with the option for a delegate when montage ends */
	void PlayAnimationMontageToOwningActor(UAnimMontage* Montage, FOnMontageEnded& MontageEndDelegate);
	void PlayAnimationMontageToOwningActor(UAnimMontage* Montage);

	/**********************************************************************************
	* Gameplay Effect Creation in Ability
	***********************************************************************************/

	/* Creates a Gameplay Effect using the current ability information (SpecHandle, ActorInfo, ActivationInfo) */
	FGameplayEffectSpecHandle MakeEffectSpecHandleFromAbility(const TSubclassOf<UGameplayEffect> EffectClass) const;

	/* Adds a 'GameplayEffect' to a target from current ability */
	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectSpecToTargetFromAbility(FGameplayEffectSpecHandle SpecHandle, FGameplayAbilityTargetDataHandle& TargetData) const;

	/* Adds a 'GameplayEffect' to the owner of the current ability */
	FActiveGameplayEffectHandle ApplyGameplayEffectSpecToOwnerFromAbility(const FGameplayEffectSpecHandle& SpecHandle) const;
};
