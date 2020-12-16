// Fill out your copyright notice in the Description page of Project Settings.


#include "ARVIIntegrationLibrary.h"

FString UARVIIntegrationLibrary::ConvertPlatformMessageDataToString(const TArray<uint8>& Data)
{
    FUTF8ToTCHAR convert((const ANSICHAR*)Data.GetData(), Data.Num());
    return FString(convert.Get());
}

TArray<uint8> UARVIIntegrationLibrary::ConvertPlatformMessageDataFromString(const FString& Message)
{
    FTCHARToUTF8 Convert(*Message);
    TArray<uint8> Data;
    Data.Append((uint8*)Convert.Get(), Convert.Length());
    return Data;
}
