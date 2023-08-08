// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitBox.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_SixParams(FHitBoxOverlapped, UPrimitiveComponent*, AActor*, UPrimitiveComponent*, int32, bool, const FHitResult&)

UCLASS()
class CRASH_API AHitBox : public AActor
{
	GENERATED_BODY()

public:
	FHitBoxOverlapped HitBoxOverlap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	UBoxComponent* HitBox;
	
	AHitBox();

	void ActivateHitBox();

	bool GetIsHitBoxActive() { return  bIsHitBoxActive; }

private:
	bool bIsHitBoxActive;
};
