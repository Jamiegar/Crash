// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Characters\CombatComponents\CombatComponent.h"
#include "Characters/CrashCharacter.h"
#include "Components/BoxComponent.h"
#include "GAS/CrashGameplayTags.h"
#include "GAS/Abiliities/Combat/Damage/KnockBackAbility.h"
#include "GAS/Abiliities/Combat/Damage/HitBoxes/HitBox.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{

}

ACrashCharacter* UCombatComponent::GetOwningCrashCharacter() const
{
	return Cast<ACrashCharacter>(GetOwner());
}

UAbilitySystemComponent* UCombatComponent::GetOwnersAbilitySystemComponent() const
{
	return GetOwningCrashCharacter()->GetAbilitySystemComponent();
}

bool UCombatComponent::PerformDamageTrace(FName BoneNameLocation)
{
	//Get the world location of socket
	const FVector SocketLocation = GetOwningCrashCharacter()->GetMesh()->GetSocketLocation(BoneNameLocation); 

#pragma region Set up SphereOverlapActors
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectFilterTypes;
	TArray<AActor*> ActorsToIgnore;
	
	ObjectFilterTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ActorsToIgnore.Add(GetOwner());

	TArray<AActor*> OverlapResult;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SocketLocation, 40.f, ObjectFilterTypes,nullptr, ActorsToIgnore, OverlapResult);
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

AHitBox* UCombatComponent::SpawnHitBox(FVector Location, FRotator Rotation)
{
	AHitBox* ActorBox = nullptr;
	
	if(UWorld* World = GetWorld())
	{
		ActorBox = World->SpawnActor<AHitBox>(AHitBox::StaticClass(), Location, Rotation);
		ActorBox->HitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &UCombatComponent::OnHitBoxOverlapWithActor);

		ActorBox->ActivateHitBox();
	}
	
	return ActorBox;
}

void UCombatComponent::OnHitBoxOverlapWithActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACrashCharacter>(OtherActor);

	if(OtherActor == GetOwner() || Character == nullptr)
		return;

	FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
	NewData->TargetActorArray.Add(OtherActor);
	const FGameplayAbilityTargetDataHandle Handle(NewData);
	
	FGameplayEventData* EventData = new FGameplayEventData();
	EventData->Instigator = GetOwningCrashCharacter();
	EventData->Target = OtherActor;
	EventData->TargetData = Handle;

	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	GetOwnersAbilitySystemComponent()->HandleGameplayEvent(GameTags.PlayerDamaged, EventData);
}

