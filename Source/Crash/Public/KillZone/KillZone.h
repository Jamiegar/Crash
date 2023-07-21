// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

class UBoxComponent;

UCLASS()
class CRASH_API AKillZone : public AActor
{
	GENERATED_BODY()

public:
	AKillZone();
	FVector GetRespawnLocation() const { return RespawnLocation + KillZoneBounds->GetComponentLocation(); }

private:

	UPROPERTY(EditDefaultsOnly, Category="Kill Zone")
	UBoxComponent* KillZoneBounds;

	UPROPERTY(EditDefaultsOnly, Category="Respawn", meta=(MakeEditWidget = true))
	FVector RespawnLocation = FVector();
	

	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
