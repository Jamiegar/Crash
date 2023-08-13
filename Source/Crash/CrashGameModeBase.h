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


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Players")
	TMap<TEnumAsByte<EAutoReceiveInput::Type>, TSubclassOf<ACrashPlayerCharacter>> CharactersToSpawn;
	
	UPROPERTY(BlueprintReadOnly, Category="Players")
	TArray<ACrashPlayerCharacter*> ActiveCharacters;
	
	virtual void BeginPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Initialisation")
	void GameSetUp();
	
	
};
