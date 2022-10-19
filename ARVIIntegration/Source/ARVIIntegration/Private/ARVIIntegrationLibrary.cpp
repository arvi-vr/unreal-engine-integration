// Copyright © 2018-2022 ARVI VR Inc.


#include "ARVIIntegrationLibrary.h"

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
