// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CrashCharacterFactory.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTOOL_API UCrashCharacterFactory : public UFactory
{
	GENERATED_BODY()

public:
	UCrashCharacterFactory();
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

private:
	TSubclassOf<class ACrashPlayerCharacter> ParentClass;
};
