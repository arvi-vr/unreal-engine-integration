// Copyright © ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "PlayAreaTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayAreaCheckingMode : uint8 {
	PACM_Auto UMETA(DisplayName = "Auto"),
	PACM_Enable UMETA(DisplayName = "Enable"),
	PACM_Disable UMETA(DisplayName = "Disable"),
};

UENUM(BlueprintType)
enum class EPlayAreaOutOfBoundsMode : uint8 {
	PAOOBM_Auto UMETA(DisplayName = "Auto"),
	PAOOBM_Block UMETA(DisplayName = "Block"),
	PAOOBM_Ignore UMETA(DisplayName = "Ignore"),
};