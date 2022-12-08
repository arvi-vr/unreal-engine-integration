// Copyright © 2018-2022 ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "AudioChatTypes.generated.h"

UENUM(BlueprintType)
enum class EAudioChatChannel : uint8 {
	ACC_Public UMETA(DisplayName = "Public"),
	ACC_Team01 UMETA(DisplayName = "Team01"),
	ACC_Team02 UMETA(DisplayName = "Team02"),
	ACC_Team03 UMETA(DisplayName = "Team03"),
	ACC_Team04 UMETA(DisplayName = "Team04"),
	ACC_Team05 UMETA(DisplayName = "Team05"),
	ACC_Team06 UMETA(DisplayName = "Team06"),
	ACC_Team07 UMETA(DisplayName = "Team07"),
	ACC_Team08 UMETA(DisplayName = "Team08"),
	ACC_Team09 UMETA(DisplayName = "Team09"),
	ACC_Team10 UMETA(DisplayName = "Team10"),
};
