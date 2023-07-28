

#include "Filters/FClassPickerViewFilter.h"

bool FClassPickerViewFilter::IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
                                            TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	if(InFilterFuncs->IfInChildOfClassesSet(AllowedClass, InClass) != EFilterReturn::Failed)
	{
		return true;
	}
	return false;
}

bool FClassPickerViewFilter::IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
	TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	return InFilterFuncs->IfInChildOfClassesSet(AllowedClass, InUnloadedClassData) != EFilterReturn::Failed;
}
