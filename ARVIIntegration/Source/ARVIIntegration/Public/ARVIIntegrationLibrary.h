// Copyright © 2018-2022 ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARVIIntegrationLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:		
	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	static FString ConvertPlatformMessageDataToString(const TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	static TArray<uint8> ConvertPlatformMessageDataFromString(const FString& Message);
};
