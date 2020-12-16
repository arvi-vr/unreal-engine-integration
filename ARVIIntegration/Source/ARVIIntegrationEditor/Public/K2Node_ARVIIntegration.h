// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_ARVIIntegration.generated.h"

/**
 * 
 */
UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_IsApplicationEntitled : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_IsApplicationEntitled(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_ServerStarted : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_ServerStarted(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_GameCompleted : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_GameCompleted(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_CallOperator : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_CallOperator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SetAudioChatChannel : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SetAudioChatChannel(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SendGameMessage : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SendGameMessage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SendLogMessage : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SendLogMessage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SendWarningMessage : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SendWarningMessage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SendTrackingMessage : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SendTrackingMessage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};


UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_ActivateInGameCommand : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_ActivateInGameCommand(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_DeactivateInGameCommand : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_DeactivateInGameCommand(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
