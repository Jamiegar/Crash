// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/CrashAbilityFactory.h"

#include "ClassViewerModule.h"
#include "KismetCompilerModule.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"
#include "CharacterTool/Public/Filters/FClassPickerViewFilter.h"

#define LOCTEXT_NAMESPACE "Crash Ability Factory" 

UCrashAbilityFactory::UCrashAbilityFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UCrashGameplayAbility::StaticClass();
}

bool UCrashAbilityFactory::ConfigureProperties()
{
	TSharedRef<FClassPickerViewFilter> ClassFilter = MakeShared<FClassPickerViewFilter>();
	ClassFilter.Get().AllowedClass.Add(UCrashGameplayAbility::StaticClass());
	
	FClassViewerInitializationOptions Options;
	Options.ClassFilters.Add(ClassFilter);
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;
	
	
	UClass* ChosenClass = nullptr;
	const FText TitleText = LOCTEXT("CreateBlueprintOptions", "Pick Parent Class");
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UCrashGameplayAbility::StaticClass());
	
	if(bPressedOk)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create New: %s"), *ChosenClass->GetName());
		ParentClass = ChosenClass;
	}
	
	return bPressedOk;
}

UObject* UCrashAbilityFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{

	if(!FKismetEditorUtilities::CanCreateBlueprintOfClass(InParent->StaticClass()))
		return nullptr;


	const IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	
	UClass* BpClass = nullptr;
	UClass* BpGenClass = nullptr;
	KismetCompilerModule.GetBlueprintTypesForClass(InParent->StaticClass(),BpClass, BpGenClass);
	
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BPTYPE_Normal, BpClass, BpGenClass, CallingContext);
}

UObject* UCrashAbilityFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(InClass, InParent, InName, Flags, Context, Warn, NAME_None);
}

#undef LOCTEXT_NAMESPACE   
