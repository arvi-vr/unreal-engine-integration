// Copyright © ARVI VR Inc.

#include "MemoryArchivingAssistant.h"
#include "ARVIIntegrationLog.h"

#include "Serialization/ArchiveUObject.h"

FMemoryReadingAssistant::FMemoryReadingAssistant(const TArray<uint8>& InBytes, bool bIsPersistent) : FMemoryReader(InBytes, bIsPersistent) 
{

}

FArchive& FMemoryReadingAssistant::operator<<(FLazyObjectPtr& Value)
{
    return FArchiveUObject::SerializeLazyObjectPtr(*this, Value);
}

FArchive& FMemoryReadingAssistant::operator<<(FSoftObjectPtr& Value)
{
    return FArchiveUObject::SerializeSoftObjectPtr(*this, Value);
}

FArchive& FMemoryReadingAssistant::operator<<(FSoftObjectPath& Value)
{
    return FArchiveUObject::SerializeSoftObjectPath(*this, Value);
}

FArchive& FMemoryReadingAssistant::operator<<(FWeakObjectPtr& Value)
{
    return FArchiveUObject::SerializeWeakObjectPtr(*this, Value);
}

FArchive& FMemoryReadingAssistant::operator<<(class UObject*& Value)
{
    UE_LOG(LogARVIIntegration, Error, TEXT("You cannot load a reference to an object"));
    return *this;
}

FMemoryWritingAssistant::FMemoryWritingAssistant(TArray<uint8>& InBytes, bool bIsPersistent, bool bSetOffset, const FName InArchiveName) : FMemoryWriter(InBytes, bIsPersistent, bSetOffset, InArchiveName)
{
}

FArchive& FMemoryWritingAssistant::operator<<(FLazyObjectPtr& Value)
{
    return FArchiveUObject::SerializeLazyObjectPtr(*this, Value);
}

FArchive& FMemoryWritingAssistant::operator<<(FSoftObjectPtr& Value)
{
    return FArchiveUObject::SerializeSoftObjectPtr(*this, Value);
}

FArchive& FMemoryWritingAssistant::operator<<(FSoftObjectPath& Value)
{
    return FArchiveUObject::SerializeSoftObjectPath(*this, Value);
}

FArchive& FMemoryWritingAssistant::operator<<(FWeakObjectPtr& Value)
{
    return FArchiveUObject::SerializeWeakObjectPtr(*this, Value);
}

FArchive& FMemoryWritingAssistant::operator<<(class UObject*& Value)
{
    UE_LOG(LogARVIIntegration, Error, TEXT("You cannot save a reference to an object"));
    return *this;
}
