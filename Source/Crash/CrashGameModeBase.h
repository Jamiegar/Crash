// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CrashCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "CrashGameModeBase.generated.h"


class ACrashCharacter;
class ACrashPlayerCharacter;


DECLARE_DELEGATE(FOnGameSetupCompleted)
inline FOnGameSetupCompleted OnGameSetupCompleted;

UCLASS()
class CRASH_API ACrashGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Players")
	int NumberOfPlayers = 2;

	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Players")
	TArray<ACrashPlayerCharacter*> ActiveCharacters;
	
	virtual void BeginPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	

private:
	UPROPERTY(EditDefaultsOnly, Category="Players")
	TMap<TEnumAsByte<EAutoReceiveInput::Type>, TSubclassOf<ACrashPlayerCharacter>> CharactersToSpawn;
	
	
};
