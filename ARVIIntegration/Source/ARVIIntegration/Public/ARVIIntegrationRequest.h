// Copyright © 2018-2022 ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AudioChatTypes.h"
#include "PlayerDominantHandTypes.h"
#include "TimerManager.h"
#include "ARVIIntegrationRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIntegrationRequestEvent, int, ErrorCode, FString, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnIntegrationRequestWithDataEvent, TArray<uint8>, Data, int, ErrorCode, FString, ErrorMessage);
/**
 * 
 */
UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintAssignable)
	FOnIntegrationRequestEvent OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnIntegrationRequestEvent OnFailed;

	UPROPERTY()
	UObject* WorldContext;

	void HandleCompleted();
	void HandleFailed(int Code, FString Message);

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration);
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "ARVI Integration")
	void Execute();
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequestWithData : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintAssignable)
	FOnIntegrationRequestWithDataEvent OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnIntegrationRequestWithDataEvent OnFailed;

	UPROPERTY()
	UObject* WorldContext;

	void HandleCompleted(const TArray<uint8>& Data);
	void HandleFailed(int Code, FString Message);

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration);
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "ARVI Integration")
	void Execute();
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_IsApplicationEntitled : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString AppKey;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:

	/**
	* Checks application entitlement
	* @param appKey	Application Key, provided with API
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "IsApplicationEntitled"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* IsApplicationEntitled(UObject* WorldContextObject, FString appKey);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_ServerStarted : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Notifies platform about starting game server
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "ServerStarted"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* ServerStarted(UObject* WorldContextObject);
};


UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_GameCompleted : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Notifies platform about completing the game
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "GameCompleted"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* GameCompleted(UObject* WorldContextObject);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_CallOperator : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Notifies platform about operator calling
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "CallOperator"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* CallOperator(UObject* WorldContextObject);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SetAudioChatChannel : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	EAudioChatChannel ChatChannel;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sets an audio chat channel
	* @param chatChannel	TeamID	Channel number. Acceptable values are TEAM01..TEAM10. Set ROOT, if you want join to default channel
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SetAudioChatChannel"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SetAudioChatChannel(UObject* WorldContextObject, EAudioChatChannel chatChannel);
};


UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SendGameMessage : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString Message;
	FName Group;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sends text message to platform
	* @note	Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 2Kb
	* @param message		Message text
	* @param group			Optional parameter. The name of the group by which the messages will be grouped
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SendGameMessage"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SendGameMessage(UObject* WorldContextObject, const FString& message, FName group = NAME_None);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SendLogMessage : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString Message;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sends service/debug message which will be logged on platform
	* @note	Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 10Kb
	* @param Message		Message text
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SendLogMessage"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SendLogMessage(UObject* WorldContextObject, const FString& message);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SendWarningMessage : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString Message;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sends warning message which will be shown in administrative panel
	* @note	Limitations: no more than 1 time per second and 10 times per minute. Message length should not exceed 2Kb
	* @param Message		Message text
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SendWarningMessage"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SendWarningMessage(UObject* WorldContextObject, const FString& message);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SendTrackingMessage : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString Message;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sends tracking message which will be tracked on log timelin
	* @note	Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 1Kb
 	* @param Message		Message text
	* @return				
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SendTrackingMessage"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SendTrackingMessage(UObject* WorldContextObject, const FString& message);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_ActivateInGameCommand : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString Message;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Activates in-game command. If several commands have the same activation message, then they will all be activated
	* @note	Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 128
	* @param ActivationMessage		Activation message
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "ActivateInGameCommand"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* ActivateInGameCommand(UObject* WorldContextObject, const FString& ActivationMessage);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_DeactivateInGameCommand : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString Message;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Deactivates in-game command. If several commands have the same deactivation message, then they will all be deactivated
	* @note	Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 128
	* @param DeactivationMessage		Deactivation message
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "DeactivateInGameCommand"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* DeactivateInGameCommand(UObject* WorldContextObject, const FString& DeactivationMessage);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SetSessionData : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString Name;
	TArray<uint8> Data;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sets session-related data by name
	* @note	Limitations: Data size should not exceed 10Mb
	* @param Name		Data name
	* @param Data		Binary data to save
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SetSessionData"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SetSessionData(UObject* WorldContextObject, const FString& Name, const TArray<uint8>& Data);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SetPlayerName : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	FString PlayerName;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sets the new player name
	* @note	Limitations: Name length should not exceed 15
	* @param NewPlayerName			New player's name
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SetPlayerName"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SetPlayerName(UObject* WorldContextObject, const FString& NewPlayerName);
};

UCLASS()
class ARVIINTEGRATION_API UARVIIntegrationRequest_SetPlayerDominantHand : public UARVIIntegrationRequest
{
	GENERATED_BODY()
protected:
	EPlayerDominantHand PlayerDominantHand;

	virtual bool ExecuteInternal(class UARVIIntegrationSubSystem* Integration) override;
public:
	/**
	* Sets the new player dominant hand
	* @param NewPlayerDominantHand		New value for the player's dominant hand
	* @return
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SetPlayerDominant"), Category = "ARVI Integration")
	static UARVIIntegrationRequest* SetPlayerDominantHand(UObject* WorldContextObject, EPlayerDominantHand NewPlayerDominantHand);
};
