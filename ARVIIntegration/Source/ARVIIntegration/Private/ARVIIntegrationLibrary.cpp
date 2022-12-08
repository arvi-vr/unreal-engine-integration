// Copyright © 2018-2022 ARVI VR Inc.


#include "ARVIIntegrationLibrary.h"
#include "ARVIIntegrationLog.h"
#include "ARVIIntegrationSubSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

FString UARVIIntegrationLibrary::ConvertPlatformMessageDataToString(const TArray<uint8>& Data)
{
    FUTF8ToTCHAR convert((const ANSICHAR*)Data.GetData(), Data.Num());
    return FString(convert.Length(), convert.Get());
}

TArray<uint8> UARVIIntegrationLibrary::ConvertPlatformMessageDataFromString(const FString& Message)
{
    FTCHARToUTF8 Convert(*Message);
    TArray<uint8> Data;
    Data.Append((uint8*)Convert.Get(), Convert.Length());
    return Data;
}

void UARVIIntegrationLibrary::TryGetSessionData(UObject* WorldContextObject, const FString& Name, ETryGetDataResult& Success, TArray<uint8>& Data)
{
    Success = ETryGetDataResult::NotFound;
    if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject)) {
        if (UARVIIntegrationSubSystem* Integration = GameInstance->GetSubsystem<UARVIIntegrationSubSystem>()) {
            if (Integration->TryGetSessionData(Name, Data))
                Success = ETryGetDataResult::Success;
        }
        else {
            UE_LOG(LogARVIIntegration, Error, TEXT("Failed to get the integration subsystem object"));
        }
    }
    else {
        UE_LOG(LogARVIIntegration, Error, TEXT("Failed to get game instance object"));
    }
}

void UARVIIntegrationLibrary::TryGetUISettingsData(UObject* WorldContextObject, const FString& Name, ETryGetDataResult& Success, FString& Value)
{
    Success = ETryGetDataResult::NotFound;
    if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject)) {
        if (UARVIIntegrationSubSystem* Integration = GameInstance->GetSubsystem<UARVIIntegrationSubSystem>()) {
            if (Integration->TryGetUISettingsData(Name, Value))
                Success = ETryGetDataResult::Success;
        }
        else {
            UE_LOG(LogARVIIntegration, Error, TEXT("Failed to get the integration subsystem object"));
        }
    }
    else {
        UE_LOG(LogARVIIntegration, Error, TEXT("Failed to get game instance object"));
    }
}
