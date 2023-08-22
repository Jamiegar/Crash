// Copyright Epic Games, Inc. All Rights Reserved.


#include "CrashGameModeBase.h"

#include "CrashGameInstance.h"
#include "Characters/CrashPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


void ACrashGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	if(UCrashGameInstance* GameInstance = Cast<UCrashGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if(GameInstance->PlayerSelection.IsEmpty())
			return;

		CharactersToSpawn.Empty();
		
		TArray<int> OutKeys;
		GameInstance->PlayerSelection.GenerateKeyArray(OutKeys);

		for (int Key : OutKeys)
		{
			TEnumAsByte<EAutoReceiveInput::Type> ID = TEnumAsByte<EAutoReceiveInput::Type>(Key + 1);
			TSubclassOf<ACrashPlayerCharacter>* Character = GameInstance->PlayerSelection.Find(Key);
			
			CharactersToSpawn.Add(ID, Character->Get());
			UE_LOG(LogTemp, Warning, TEXT("Added: %s to Player: %i"), *Character->Get()->GetName(), ID.GetValue());
		}
	}
}

void ACrashGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	
	for(int i = 1; i < CharactersToSpawn.Num(); i++)
	{
		UGameplayStatics::CreatePlayer(this, i, true);
	}
	
	OnGameSetupCompleted.ExecuteIfBound();

	GameSetUp();

	for (const auto Character : ActiveCharacters)
	{
		Character->OnCharacterDeath.AddUniqueDynamic(this, &ACrashGameModeBase::OnCharacterDeath);
		Character->OnCharacterKnockedOut.AddUniqueDynamic(this, &ACrashGameModeBase::OnCharacterKO);
	}
}

UClass* ACrashGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if(APlayerController* PlayerController = Cast<APlayerController>(InController))
	{
		const int32 ID = UGameplayStatics::GetPlayerControllerID(PlayerController);
		
		if(const TSubclassOf<ACrashPlayerCharacter>* CharacterToSpawn = CharactersToSpawn.Find(TEnumAsByte<EAutoReceiveInput::Type>(ID + 1)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller ID: %i Assinged to: %s"), ID, *CharacterToSpawn->Get()->GetName());
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

void ACrashGameModeBase::OnCharacterDeath()
{
	for (const auto Character : ActiveCharacters)
	{
		Character->ApplyDynamicForceFeedback(1, 1.0f);
	}
	
}

void ACrashGameModeBase::OnCharacterKO(ACrashCharacter* KnockedOutCharacter)
{
	if(KnockedOutCharacter != ActiveCharacters[0])
	{
		if(APlayerController* Controller = ActiveCharacters[0]->GetCrashCharacterPlayerController())
			OnCharacterKnockedOut(Controller);
	}
	else
	{
		if(APlayerController* Controller = ActiveCharacters[1]->GetCrashCharacterPlayerController())
			OnCharacterKnockedOut(Controller);
	}

	for (const auto Character : ActiveCharacters)
	{
		Character->bCanMove = false;
		
	}
}
