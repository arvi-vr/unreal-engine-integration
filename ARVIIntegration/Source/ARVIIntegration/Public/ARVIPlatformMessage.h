// Copyright © 2018-2022 ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "ARVIPlatformMessage.generated.h"

USTRUCT(BlueprintType)
struct ARVIINTEGRATION_API FARVIPlatformMessageParam {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
		FString Value;

	FARVIPlatformMessageParam() :Name(), Value() {

	}

	FARVIPlatformMessageParam(FString name, FString value) : Name(name), Value(value) {

	}
};

USTRUCT(BlueprintType)
struct ARVIINTEGRATION_API FARVIPlatformMessage {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FString Method;

	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	TArray<FARVIPlatformMessageParam> Params;

	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	TArray<uint8> Data;

	FARVIPlatformMessage() : Name(), Method(), Params(), Data() {
	
	}
};
