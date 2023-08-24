// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/CrashGameplayTags.h"
#include "GAS/Effects/DamageBasicInstant.h"
#include "Crash/Public/GAS/Abiliities/Combat/Damage/Data/StunAbilityData.h"
#include "GAS/Effects/KnockbackCalculationEffect.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Characters/CrashCharacter.h"
#include "GAS/Abiliities/Combat/Damage/StunAbility.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"
#include "GAS/Effects/Damaging/HitStopEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/CameraSubsystem.h"

UAttackAbility::UAttackAbility()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));

	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.Combo"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));

	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultMissedSoundEffect
		(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Blueprints/MetaSounds/CharacterAttacks/MS_AttackNoContactWhoost.MS_AttackNoContactWhoost'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultContactSoundEffect
			(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Blueprints/MetaSounds/CharacterAttacks/MS_AttackContactHit.MS_AttackContactHit'"));
	
	SoundData.MissedAttackSound = DefaultMissedSoundEffect.Object;
	SoundData.ContactHitSound = DefaultContactSoundEffect.Object;
}

void UAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if(AsyncDamageTask)
		if(AsyncDamageTask->IsActive())
			AsyncDamageTask->EndTask();
}

void UAttackAbility::WaitForDamageEffect()
{
	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	AsyncDamageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameTags.PlayerDamaged, nullptr, true);
	AsyncDamageTask->EventReceived.AddUniqueDynamic(this, &UAttackAbility::OnGameplayReceivedDamageEvent);
	AsyncDamageTask->Activate();
}

void UAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	PayLoadEventData = Payload;

	ACrashCharacter* OwnerCharacter = CastChecked<ACrashCharacter>(GetActorInfo().OwnerActor);
	ACrashCharacter* TargetCharacter = Cast<ACrashCharacter>(Payload.Target);

	if(!TargetCharacter)
		return;

	if(TargetCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Player.State.Invincible")) && !DoesTargetHaveCounterAttack(TargetCharacter))
		return;

	
	UCameraSubsystem* CameraSubsystem = GetWorld()->GetSubsystem<UCameraSubsystem>();
	const float Trauma = (AbilityDamage / 20) * 3.5;
	CameraSubsystem->ApplyCameraShake(Trauma, 2, 400, 75, 0.5, 1.5);

	OwnerCharacter->ApplyDynamicForceFeedback(1.0f, 0.2f);
	TargetCharacter->ApplyDynamicForceFeedback(1.0f, 0.2f);
	
	TargetCharacter->FaceActor(GetActorInfo().OwnerActor.Get());

	if(DoesTargetHaveCounterAttack(TargetCharacter))
	{
		//Counter attacks owner if target is counter attacking
		WaitForHitStopEndAndApplyCounter(Payload);
	}
	else
	{
		//Perform Normal Damage 
		const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
		const FGameplayEffectSpecHandle Handle = MakeEffectSpecHandleFromAbility(UDamageBasicInstant::StaticClass());

		if(!Handle.IsValid())
			return;
		
		FGameplayEffectSpec* Spec = Handle.Data.Get();
		Spec->SetSetByCallerMagnitude(GameTags.PlayerDamaged, AbilityDamage); //Sends the Ability damage to the custom damage execute calculation 
		ApplyAbilityTagsToGameplayEffectSpec(*Handle.Data.Get(), GetCurrentAbilitySpec()); 
		ApplyGameplayEffectSpecToTargetFromAbility(Handle, Payload.TargetData);
	}
}

bool UAttackAbility::DoesTargetHaveCounterAttack(ACrashCharacter* Target)
{
	if(!Target)
		return false;

	const FGameplayTag CounterTag = FGameplayTag::RequestGameplayTag("Player.State.Counter");
	return Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(CounterTag);
}

void UAttackAbility::ApplyKnockbackInstantToTarget(FGameplayEventData Payload)
{
	const ACrashCharacter* TargetCharacter = Cast<ACrashCharacter>(Payload.Target);

	if(!TargetCharacter)
		return;
	
	if(TargetCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Player.State.Invincible")))
		return;
	
	PayLoadEventData = Payload;
	
	const FGameplayEffectSpecHandle Handle = MakeEffectSpecHandleFromAbility(UKnockbackCalculationEffect::StaticClass());
	
	FGameplayEffectSpec* Spec = Handle.Data.Get();
	Spec->SetSetByCallerMagnitude("Player.Damaged.Knockback", KnockbackScaling);

	ApplyAbilityTagsToGameplayEffectSpec(*Handle.Data.Get(), GetCurrentAbilitySpec());
	
	auto ActiveGameplayEffectHandles= ApplyGameplayEffectSpecToTargetFromAbility(Handle, Payload.TargetData);

	FGameplayEventData EventKnockbackData;
	EventKnockbackData.Instigator = GetActorInfo().AvatarActor.Get();
	EventKnockbackData.Target = Payload.Target;

	if(KnockbackData)
		EventKnockbackData.OptionalObject = KnockbackData;
		
	UAbilitySystemComponent* TargetComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Payload.Target);
	TargetComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Data.Knockback"), &EventKnockbackData);
	
}

void UAttackAbility::ApplyKnockbackInstantToTarget()
{
	ApplyKnockbackInstantToTarget(PayLoadEventData);
}

void UAttackAbility::ApplyStunToTarget(FGameplayEventData Payload)
{
	PayLoadEventData = Payload;
	
	UAbilitySystemComponent* TargetComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Payload.Target);

	FGameplayAbilitySpec StunSpec = FGameplayAbilitySpec(UStunAbility::StaticClass());
	TargetComponent->GiveAbility(StunSpec);
	
	if(StunData != nullptr && bAttackShouldStun)
	{
		FGameplayEventData StunPayloadData;
		StunPayloadData.Instigator = CastChecked<AActor>(GetActorInfo().AvatarActor);
		StunPayloadData.Target = Payload.Target;
		StunPayloadData.OptionalObject = StunData;

		const FGameplayTag StunEventTag = FGameplayTag::RequestGameplayTag("Event.Data.Stun");
		TargetComponent->HandleGameplayEvent(StunEventTag, &StunPayloadData);
	}
}

void UAttackAbility::ApplyHitStopInstant(FGameplayEventData Payload) 
{
	PayLoadEventData = Payload;
	
	const FGameplayEffectSpecHandle HitStopHandle = MakeEffectSpecHandleFromAbility(UHitStopEffect::StaticClass());
	const float HitStopDuration = (AbilityDamage / 20) * 2; 
	HitStopHandle.Data.Get()->SetDuration(HitStopDuration, true);
	
	if(!HitStopHandle.IsValid())
		return;

	ApplyAbilityTagsToGameplayEffectSpec(*HitStopHandle.Data.Get(), GetCurrentAbilitySpec());

	ApplyGameplayEffectSpecToTargetFromAbility(HitStopHandle, Payload.TargetData);
	auto ActiveGameplayEffectHandles = ApplyGameplayEffectSpecToOwnerFromAbility(HitStopHandle);
}


void UAttackAbility::WaitForHitStopEndAndApplyCounter(FGameplayEventData Payload)
{
	PayLoadEventData = Payload;
	
	ApplyHitStopInstant(Payload);

	FGameplayTag HitStopTag = FGameplayTag::RequestGameplayTag("Player.Damaged.HitStop");
	AsyncHitStopTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, HitStopTag, nullptr, true);
	AsyncHitStopTagRemoved->Removed.AddUniqueDynamic(this, &UAttackAbility::OnHitStopEndApplyCounter);
	AsyncHitStopTagRemoved->Activate();
}

void UAttackAbility::OnHitStopEndApplyCounter()
{
	FGameplayEventData CounterData;
	CounterData.Instigator = PayLoadEventData.Target;
	CounterData.Target = PayLoadEventData.Instigator;

	UAbilitySystemComponent* TargetComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PayLoadEventData.Target);
	const FGameplayTag PerformCounterTag = FGameplayTag::RequestGameplayTag("Event.Counter");
	TargetComponent->HandleGameplayEvent(PerformCounterTag, &CounterData);
}

void UAttackAbility::WaitForHitStopEndAndApplyKnockback(FGameplayEventData Payload)
{
	PayLoadEventData = Payload;
	
	ApplyHitStopInstant(Payload);

	FGameplayTag HitStopTag = FGameplayTag::RequestGameplayTag("Player.Damaged.HitStop");
	AsyncHitStopTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, HitStopTag, nullptr, true);
	AsyncHitStopTagRemoved->Removed.AddUniqueDynamic(this, &UAttackAbility::OnHitStopEndApplyKnockback);
	AsyncHitStopTagRemoved->Activate();
}

void UAttackAbility::OnHitStopEndApplyKnockback()
{
	ApplyKnockbackInstantToTarget();
}

void UAttackAbility::PlayMissedAttackSound()
{
	PlaySoundAtOwnerLocation(SoundData.MissedAttackSound);
}

void UAttackAbility::PlayContactHitAttackSound()
{
	PlaySoundAtOwnerLocation(SoundData.ContactHitSound);
}

UAudioComponent* UAttackAbility::PlaySoundAtOwnerLocation(USoundBase* Sound)
{
	const AActor* OwnerTransform = GetActorInfo().OwnerActor.Get();
	return UGameplayStatics::SpawnSoundAtLocation(OwnerTransform->GetWorld(), Sound, OwnerTransform->GetActorLocation(), FRotator::ZeroRotator);
}
