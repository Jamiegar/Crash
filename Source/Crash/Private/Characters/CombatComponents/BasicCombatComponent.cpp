// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CombatComponents/BasicCombatComponent.h"
#include "Characters/CrashCharacter.h"
#include "GAS/CrashGameplayTags.h"
#include "GAS/Abiliities/Combat/Basic/ComboBasic.h"
#include "GAS/Abiliities/Combat/Basic/DownBasic.h"
#include "GAS/Abiliities/Combat/Basic/MiddleBasic.h"
#include "GAS/Abiliities/Combat/Basic/UpBasic.h"
#include "GAS/Abiliities/Combat/Damage/KnockBackAbility.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UBasicCombatComponent::UBasicCombatComponent()
{
	BasicCombatAbilities.Add(UMiddleBasic::StaticClass());
	BasicCombatAbilities.Add(UUpBasic::StaticClass());
	BasicCombatAbilities.Add(UDownBasic::StaticClass());
	BasicCombatAbilities.Add(UComboBasic::StaticClass());
}

ACrashCharacter* UBasicCombatComponent::GetOwningCrashCharacter() const
{
	return Cast<ACrashCharacter>(GetOwner());
}

UAbilitySystemComponent* UBasicCombatComponent::GetOwnersAbilitySystemComponent() const
{
	return GetOwningCrashCharacter()->GetAbilitySystemComponent();
}

bool UBasicCombatComponent::PerformDamageTrace(FName BoneNameLocation)
{
	//Get the world location of socket
	const FVector SocketLocation = GetOwningCrashCharacter()->GetMesh()->GetSocketLocation(BoneNameLocation); 

#pragma region Set up SphereOverlapActors
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectFilterTypes;
	TArray<AActor*> ActorsToIgnore;
	
	ObjectFilterTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ActorsToIgnore.Add(GetOwner());

	TArray<AActor*> OverlapResult;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SocketLocation, 40.f, ObjectFilterTypes,
		nullptr, ActorsToIgnore, OverlapResult);
#pragma endregion
	
	if(OverlapResult.IsValidIndex(0))
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Add(OverlapResult[0]);
		const FGameplayAbilityTargetDataHandle Handle(NewData);

		FGameplayEventData* EventData = new FGameplayEventData();
		EventData->Instigator = GetOwningCrashCharacter();
		EventData->Target = OverlapResult[0];
		EventData->TargetData = Handle;

		const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
		GetOwnersAbilitySystemComponent()->HandleGameplayEvent(GameTags.PlayerDamaged, EventData);
		return true;
	}
	return false;
}

