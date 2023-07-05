// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboAbilityData.generated.h"


UCLASS()
class CRASH_API UComboAbilityData : public UDataAsset
{
	GENERATED_BODY()

public:
	UComboAbilityData() {}
	UComboAbilityData(int ComboCounter);
	int GetComboCount() const { return ComboCount; }
	
private:
	int ComboCount;
};
