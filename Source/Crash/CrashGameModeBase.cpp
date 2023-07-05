// Copyright Epic Games, Inc. All Rights Reserved.


#include "CrashGameModeBase.h"
#include "Characters/CrashPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ACrashGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 1; i < NumberOfPlayers; i++)
	{
		UGameplayStatics::CreatePlayer(this, 1, true);
	}
}

void ACrashGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if(ACrashPlayerCharacter* PlayerPawn = Cast<ACrashPlayerCharacter>(NewPlayer->GetPawn()))
	{
		PlayerPawn->InitializePlayerCharacter();
	}
}
