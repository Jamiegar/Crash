// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrashGlobalFunctions.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UCrashGlobalFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void CreateObjectInContentBrowser();
};
