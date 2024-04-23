// Copyright © ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "PlayerDominantHandTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerDominantHand : uint8 {
	PDH_NotSet UMETA(DisplayName = "NotSet"),
	PDH_Left UMETA(DisplayName = "Left"),
	PDH_Right UMETA(DisplayName = "Right"),
	PDH_Any UMETA(DisplayName = "Any")
};