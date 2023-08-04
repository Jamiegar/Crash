// Copyright Epic Games, Inc. All Rights Reserved.


#include "CrashGameModeBase.h"
#include "Characters/CrashPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ACrashGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 1; i < CharactersToSpawn.Num(); i++)
	{
		APlayerController* Controller = UGameplayStatics::CreatePlayer(this, i, true);
	}

	OnGameSetupCompleted.ExecuteIfBound();
}

UClass* ACrashGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if(APlayerController* PlayerController = Cast<APlayerController>(InController))
	{
		const int32 ID = UGameplayStatics::GetPlayerControllerID(PlayerController);
		
		if(const TSubclassOf<ACrashPlayerCharacter>* CharacterToSpawn = CharactersToSpawn.Find(TEnumAsByte<EAutoReceiveInput::Type>(ID + 1)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller ID: %i"), ID);
			return CharacterToSpawn->Get();
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ACrashGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if(ACrashPlayerCharacter* PlayerPawn = Cast<ACrashPlayerCharacter>(NewPlayer->GetPawn()))
	{
		ActiveCharacters.Add(PlayerPawn);
		PlayerPawn->InitializePlayerCharacter();
	}
}
