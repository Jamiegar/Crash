// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/CrashCharacterFactory.h"

#include "ClassViewerModule.h"
#include "KismetCompilerModule.h"
#include "CharacterTool/Public/Filters/FClassPickerViewFilter.h"
#include "Characters/CrashPlayerCharacter.h"
#include "ContentBrowser/Private/SContentBrowser.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"

#define LOCTEXT_NAMESPACE "Crash Character Factory"
UCrashCharacterFactory::UCrashCharacterFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = ACrashPlayerCharacter::StaticClass();
}

bool UCrashCharacterFactory::ConfigureProperties()
{
	TSharedRef<FClassPickerViewFilter> ClassFilter = MakeShared<FClassPickerViewFilter>();
	ClassFilter.Get().AllowedClass.Add(ACrashPlayerCharacter::StaticClass());

	FClassViewerInitializationOptions Options;
	Options.ClassFilters.Add(ClassFilter);
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;
	
	UClass* ChosenClass = nullptr;
	const FText TitleText = LOCTEXT("CreateBlueprintOptions", "Pick Parent Class");
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, ACrashPlayerCharacter::StaticClass());
	
	if(bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	
	
	return bPressedOk;
}

UObject* UCrashCharacterFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	if(!FKismetEditorUtilities::CanCreateBlueprintOfClass(InParent->StaticClass()))
		return nullptr;


	const IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	
	UClass* BpClass = nullptr;
	UClass* BpGenClass = nullptr;
	KismetCompilerModule.GetBlueprintTypesForClass(InParent->StaticClass(),BpClass, BpGenClass);
	
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BPTYPE_Normal, BpClass, BpGenClass, CallingContext);
}

UObject* UCrashCharacterFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(InClass, InParent, InName, Flags, Context, Warn, NAME_None);
}
#undef LOCTEXT_NAMESPACE