// Copyright © ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

class ARVIINTEGRATION_API FMemoryReadingAssistant : public FMemoryReader {

public:
	FMemoryReadingAssistant(const TArray<uint8>& InBytes, bool bIsPersistent = false);

	virtual FArchive& operator<<(FLazyObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPath& Value) override;
	virtual FArchive& operator<<(FWeakObjectPtr& Value) override;
	virtual FArchive& operator<<(class UObject*& Value) override;
};

class ARVIINTEGRATION_API FMemoryWritingAssistant : public FMemoryWriter {

public:
	FMemoryWritingAssistant(TArray<uint8>& InBytes, bool bIsPersistent = false, bool bSetOffset = false, const FName InArchiveName = NAME_None);

	virtual FArchive& operator<<(FLazyObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPath& Value) override;
	virtual FArchive& operator<<(FWeakObjectPtr& Value) override;
	virtual FArchive& operator<<(class UObject*& Value) override;
};
