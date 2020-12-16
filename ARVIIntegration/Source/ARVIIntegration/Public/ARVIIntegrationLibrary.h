// Fill out your copyright notice in the Description page of Project Settings.

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
