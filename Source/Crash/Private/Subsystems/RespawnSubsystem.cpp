// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/RespawnSubsystem.h"

#include "Characters/CrashCharacter.h"
#include "KillZone/KillZone.h"
#include "KillZone/RespawnAnchor.h"
#include "Kismet/GameplayStatics.h"

void URespawnSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	KillZone = CastChecked<AKillZone>(UGameplayStatics::GetActorOfClass(GetWorld(), AKillZone::StaticClass()));
}

void URespawnSubsystem::StartRespawnSequence(ACrashCharacter* Character) const
{
	const FVector Location = KillZone->GetRespawnLocation();
	const FRotator Rotation = FRotator();
	const FActorSpawnParameters SpawnInfo;
	AActor* ActorRespawnPad = GetWorld()->SpawnActor<ARespawnAnchor>(Location, Rotation, SpawnInfo);

	ARespawnAnchor* RespawnPad = CastChecked<ARespawnAnchor>(ActorRespawnPad);
	RespawnPad->RespawnCharacter(Character, KillZone->GetRespawnLocation());
}
