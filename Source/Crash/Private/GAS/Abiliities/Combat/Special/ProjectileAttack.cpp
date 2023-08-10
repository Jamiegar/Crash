// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Special/ProjectileAttack.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/Interfaces/CombatAbilities.h"
#include "GAS/Abiliities/Combat/Projectiles/Projectile.h"
#include "GAS/Effects/DamageBasicInstant.h"
#include "Kismet/KismetMathLibrary.h"

UProjectileAttack::UProjectileAttack()
{
	
}

void UProjectileAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		return;

	if(!ProjectileAttackMontage || !Projectile)
		return;

	const FGameplayTag FireProjectileTag = FGameplayTag::RequestGameplayTag("Event.Montage.FireProjectile");
	AsyncProjectileFiredEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FireProjectileTag, nullptr, true);
	AsyncProjectileFiredEvent->EventReceived.AddUniqueDynamic(this, &UProjectileAttack::OnGameplayReceivedFireProjectile);
	AsyncProjectileFiredEvent->Activate();

	FOnMontageEnded Delegate;
	Delegate.BindUObject(this, &UProjectileAttack::OnMontageFinished);
	PlayAnimationMontageToOwningActor(ProjectileAttackMontage, Delegate);
}

void UProjectileAttack::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	EndCrashAbility();
}

void UProjectileAttack::OnGameplayReceivedFireProjectile(FGameplayEventData Payload)
{
	SpawnProjectile();
}

void UProjectileAttack::SpawnProjectile()
{
	if(ICombatAbilities* CombatAbilities = Cast<ICombatAbilities>(GetActorInfo().OwnerActor))
	{
		if(!Projectile)
			return;
		
		const AActor* Owner = GetActorInfo().OwnerActor.Get();

		const FVector Location = CombatAbilities->GetProjectileSpawnTransform().GetLocation();
		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100));
		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Cast<APawn>(GetActorInfo().OwnerActor.Get());

		if(AProjectile* ProjectileObj = GetWorld()->SpawnActor<AProjectile>(Projectile, Location, Rotation, SpawnInfo))
			ProjectileObj->SetDamagingEffectHandle(MakeOutgoingGameplayEffectSpec(UDamageBasicInstant::StaticClass()));
		
	}
}



