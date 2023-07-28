// Fill out your copyright notice in the Description page of Project Settings.


#include "Menus/AbilityMapping/InputAbilityMappingWidget.h"
#include "ClassViewerModule.h"
#include "SlateOptMacros.h"
#include "Filters/FClassPickerViewFilter.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInputAbilityMappingWidget::Construct(const FArguments& InArgs)
{
	OnAbilitySelected = InArgs._OnAbilitySelected;
	WidgetInputActionType = InArgs._Action;
	
	ChildSlot
	[
		SNew(SComboButton)
		.ButtonContent()
		[
			SNew(SImage)
			.Image(new FSlateDynamicImageBrush(InArgs._InputImage, FVector2D(64, 64)))
		]
		.OnGetMenuContent(this, &SInputAbilityMappingWidget::OnMappingButtonClicked)
	];
}

TSharedRef<SWidget> SInputAbilityMappingWidget::OnMappingButtonClicked()
{
	
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	const TSharedRef<FClassPickerViewFilter> ClassFilter = MakeShared<FClassPickerViewFilter>();
	ClassFilter.Get().AllowedClass.Add(UCrashGameplayAbility::StaticClass());  

	FClassViewerInitializationOptions Options;
	Options.ClassFilters.Add(ClassFilter);
	Options.Mode = EClassViewerMode::Type::ClassPicker;

	const TSharedRef<SWidget> ClassViewer = ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateRaw(this, &SInputAbilityMappingWidget::OnClassPicked));
	
	return
		SNew(SBox)
		.HeightOverride(300)
		.WidthOverride(300)
		[
			ClassViewer
		];
}

void SInputAbilityMappingWidget::OnClassPicked(UClass* SelectedClass)
{
	OnAbilitySelected.ExecuteIfBound(SelectedClass->GetDefaultObject<UCrashGameplayAbility>(), WidgetInputActionType);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
