// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CrashAssetManager.generated.h"

UCLASS()
class CRASH_API UCrashAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UCrashAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
