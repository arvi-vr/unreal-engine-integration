// Copyright © 2018-2022 ARVI VR Inc.


#include "ARVIIntegrationLibrary.h"
#include "ARVIIntegrationLog.h"
#include "ARVIIntegrationSubSystem.h"

#include "MemoryArchivingAssistant.h"

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

void UARVIIntegrationLibrary::TryGetSessionValue(UObject* WorldContextObject, const FString& Name, ETryGetValueResult& Success, FGenericStruct& Value)
{
    
}

DEFINE_FUNCTION(UARVIIntegrationLibrary::execTryGetSessionValue){
    P_GET_OBJECT(UObject, WorldContextObject);
    P_GET_STRUCT(FString, Name);
    P_GET_ENUM_REF(ETryGetValueResult, Success)

    Stack.StepCompiledIn<FProperty>(NULL);
    FProperty* Property = Stack.MostRecentProperty;
    void* SrcStructAddr = Stack.MostRecentPropertyAddress;


    P_FINISH;

   

    P_NATIVE_BEGIN;
    TArray<uint8> Buff;
    ETryGetDataResult GetDataResult;
    TryGetSessionData(WorldContextObject, Name, GetDataResult, Buff);
    switch (GetDataResult)
    {
    case ETryGetDataResult::Success:
    {        
        FMemoryReadingAssistant Reader(Buff);

        Property->SerializeItem(FStructuredArchiveFromArchive(Reader).GetSlot(), SrcStructAddr);
        Success = Reader.IsError() ? ETryGetValueResult::Fail : ETryGetValueResult::Success;
        
        break; 
    }
    case ETryGetDataResult::NotFound:
    default:
        Success = ETryGetValueResult::NotFound;
        break;
    }
    P_NATIVE_END;
}

void UARVIIntegrationLibrary::ConvertToSessionData(TArray<uint8>& Data, const FGenericStruct& Value)
{
}

DEFINE_FUNCTION(UARVIIntegrationLibrary::execConvertToSessionData) {
    P_GET_TARRAY_REF(uint8, Data);

    Stack.StepCompiledIn<FProperty>(NULL);
    FProperty* Property = Stack.MostRecentProperty;
    void* SrcStructAddr = Stack.MostRecentPropertyAddress;


    P_FINISH;

    P_NATIVE_BEGIN;
    FMemoryWritingAssistant Writer(Data);
    Property->SerializeItem(FStructuredArchiveFromArchive(Writer).GetSlot(), SrcStructAddr);
    if (Writer.IsError()) {
        UE_LOG(LogARVIIntegration, Warning, TEXT("Failed to convert session value"));
    }
    P_NATIVE_END;
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
