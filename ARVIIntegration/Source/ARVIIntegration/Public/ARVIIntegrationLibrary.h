// Copyright © 2018-2022 ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARVIIntegrationLibrary.generated.h"

UENUM()
enum class ETryGetDataResult : uint8 {
	Success  UMETA(DisplayName = "Success"),
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

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = Success, WorldContext = WorldContextObject), Category = "ARVI Integration")
	static void TryGetUISettingsData(UObject* WorldContextObject, const FString& Name, ETryGetDataResult& Success, FString& Value);
public:		
	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	static FString ConvertPlatformMessageDataToString(const TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	static TArray<uint8> ConvertPlatformMessageDataFromString(const FString& Message);
};
