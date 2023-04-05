// Copyright © 2018-2022 ARVI VR Inc.

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
class ARVIINTEGRATIONEDITOR_API UK2Node_ActivateInGameCommands : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_ActivateInGameCommands(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_DeactivateInGameCommand : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_DeactivateInGameCommand(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_DeactivateInGameCommands : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_DeactivateInGameCommands(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SetSessionData : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SetSessionData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SetPlayerName : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SetPlayerName(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARVIINTEGRATIONEDITOR_API UK2Node_SetPlayerDominantHand : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
public:
	UK2Node_SetPlayerDominantHand(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};