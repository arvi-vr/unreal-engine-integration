// Copyright © 2018-2022 ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ARVIIntegrationLibrary.generated.h"

UENUM()
enum class ETryGetDataResult : uint8 {
	Success  UMETA(DisplayName = "Success"),
	NotFound  UMETA(DisplayName = "Not found"),
};

UENUM()
enum class ETryGetValueResult : uint8 {
	Success  UMETA(DisplayName = "Success"),
	Fail  UMETA(DisplayName = "Fail"),
	NotFound  UMETA(DisplayName = "Not found"),	
};

/**
 * 
 */
UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = Success, WorldContext = WorldContextObject), Category = "ARVI Integration")
	static void TryGetSessionData(UObject* WorldContextObject, const FString& Name, ETryGetDataResult& Success, TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, CustomThunk, Meta = (ExpandEnumAsExecs = Success, WorldContext = "WorldContextObject", CustomStructureParam = "Value"), Category = "ARVI Integration")
	static void TryGetSessionValue(UObject* WorldContextObject, const FString& Name, ETryGetValueResult& Success, FGenericStruct& Value);

	DECLARE_FUNCTION(execTryGetSessionValue);

	UFUNCTION(BlueprintCallable, CustomThunk, Meta = (CustomStructureParam = "Value"), Category = "ARVI Integration")
	static void ConvertToSessionData(TArray<uint8>& Data, const FGenericStruct& Value);

	DECLARE_FUNCTION(execConvertToSessionData);

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = Success, WorldContext = WorldContextObject), Category = "ARVI Integration")
	static void TryGetUISettingsData(UObject* WorldContextObject, const FString& Name, ETryGetDataResult& Success, FString& Value);
public:		
	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	static FString ConvertPlatformMessageDataToString(const TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	static TArray<uint8> ConvertPlatformMessageDataFromString(const FString& Message);
};
