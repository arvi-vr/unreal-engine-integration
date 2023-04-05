// Copyright © 2018-2022 ARVI VR Inc.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioChatTypes.h"
#include "PlayerDominantHandTypes.h"
#include "ARVIPlatformMessage.h"
#include "MemoryArchivingAssistant.h"
#include "ARVIIntegrationSubSystem.generated.h"

DECLARE_DELEGATE(FOnARVIIntegrationRequestCompleted);
DECLARE_DELEGATE_TwoParams(FOnARVIIntegrationRequestFailed, int, FString);
DECLARE_DELEGATE_OneParam(FOnARVIIntegrationRequestCompletedWithData, const TArray<uint8>& );

[[deprecated("Use FOnARVIIntegrationRequestFailed instead.")]]
typedef FOnARVIIntegrationRequestFailed FOnARVIIntegrationRequesFailed;

[[deprecated("Use FOnARVIIntegrationRequestCompleted instead.")]]
typedef FOnARVIIntegrationRequestCompleted FOnARVIIntegrationRequesCompleted;
                          
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FPlayerPositionRequestDelegate, FTransform&, PlayerTransform);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FTimeLefRequestDelegate, int&, TimeLeft);
DECLARE_DYNAMIC_DELEGATE_RetVal_ThreeParams(bool, FPlatformMessageRequestDelegate, const FARVIPlatformMessage&, Message, FString&, ContentType, TArray<uint8>&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerNameChangeNotificationDelegate, const FString&, NewPlayerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDominantHandChangeNotificationDelegate, EPlayerDominantHand, NewDominantHand);

USTRUCT()
struct ARVIINTEGRATION_API FRequestCallback {
	GENERATED_BODY()
	FOnARVIIntegrationRequestCompleted OnCompleted;
	FOnARVIIntegrationRequestFailed OnFailed;
	FOnARVIIntegrationRequestCompletedWithData OnCompletedWithData;

	FRequestCallback() : OnCompleted(nullptr), OnFailed(nullptr), OnCompletedWithData(nullptr) {
	
	}

	FRequestCallback(FOnARVIIntegrationRequestCompleted onCompleted, FOnARVIIntegrationRequestFailed onFailed) : OnCompleted(onCompleted), OnFailed(onFailed), OnCompletedWithData(nullptr) {

	}

	FRequestCallback(FOnARVIIntegrationRequestCompletedWithData onCompletedWithData, FOnARVIIntegrationRequestFailed onFailed) : OnCompleted(nullptr), OnFailed(onFailed), OnCompletedWithData(onCompletedWithData) {

	}
};

/**
 * 
 */
UCLASS(Config = Engine)
class ARVIINTEGRATION_API UARVIIntegrationSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/**
	* Player position request handler
	*/
	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FPlayerPositionRequestDelegate PlayerPositionRequestHandler;

	/**
	* Remaining game time request handler
	*/
	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FTimeLefRequestDelegate TimeLeftRequestHandler;

	/**
	* Arbitrary request handler	
	*/
	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FPlatformMessageRequestDelegate PlatformMessageRequestHandler;

	/**
	* Player name change handler
	*/
	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FPlayerNameChangeNotificationDelegate PlayerNameChangedHandler;

	/**
	* Player dominant hand change handler
	*/
	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FPlayerDominantHandChangeNotificationDelegate PlayerDominantHandChangedHandler;
protected:
	/**
	* Indicates if the module Messages has been initialized
	*/
	UPROPERTY(BlueprintReadOnly, Category = "ARVI Integration")
	bool bMessagesWasInitialize;

	/**
	* Indicates if the module SessionVariables has been initialized
	*/
	UPROPERTY(BlueprintReadOnly, Category = "ARVI Integration")
	bool bSessionVariablesWasInitialize;

	/**
	* Gets if the game should track headset cord twisting
	*/
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetShouldApplicationTrackCordTwisting, Category = "ARVI Integration")
	bool bShouldApplicationTrackCordTwisting;

	/**
	* Gets if the application is running in trial mode
	*/
	UPROPERTY(BlueprintReadOnly,BlueprintGetter=GetIsApplicationInTrialMode, Category = "ARVI Integration")
	bool bIsApplicationInTrialMode;

	/**
	* Number of players in the session
	*/
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetPlayersCount, Category = "ARVI Integration")
	int PlayersCount;

	/**
	* Game server IP address
	*/
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetServerIP, Category = "ARVI Integration")
	FString ServerIP;

	/**
	* Game session language
	*/
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetSessionLanguage, Category = "ARVI Integration")
	FString SessionLanguage;

	/**
	* Game session time in seconds
	*/
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetSessionTime, Category = "ARVI Integration")
	int SessionTime;

	/**
	* Unique game session identifier. 
	* Can be useful for separating sessions if there are several sessions of your game running on the network and you are looking for players on the network yourself
	*/
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetSessionID, Category = "ARVI Integration")
	FString SessionID;

	/**
	* Player's ID
	*/
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetPlayerID, Category = "ARVI Integration")
	FString PlayerID;

	/**
	* Player's name
	*/
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetPlayerName, Category = "ARVI Integration")
	FString PlayerName;

	/**
	* Player's dominant hand
	*/
	UPROPERTY(BlueprintReadWrite, BlueprintGetter=GetPlayerDominantHand, Category = "ARVI Integration")
	EPlayerDominantHand PlayerDominantHand;

	UPROPERTY()
	TMap<int64, FRequestCallback> Requests;
	
	UPROPERTY(Config)
	int MaxResponsesPerCall;

	UPROPERTY(Config)
	int MaxMessagesPerCall;

	UPROPERTY(Config)
	float RequestTimeInterval;

	UPROPERTY(Config)
	float MessageTimeInterval;

	FTimerHandle RequestCompletedTimer;
	FTimerHandle MessageRequestTimer;
	
	void RequestCompletedWorker();
	void MessageRequestWorker();

	bool DispatchPlayerPositionRequest(const FARVIPlatformMessage& Message, FString& ContentType, TArray<uint8>& Data);
	bool DispatchTimeLeftRequest(const FARVIPlatformMessage& Message, FString& ContentType, TArray<uint8>& Data);
	bool DispatchPlatformMessage(const FARVIPlatformMessage& Message, FString& ContentType, TArray<uint8>& Data);

	void DispatchEventPlayerNameChanged();
	void DispatchEventPlayerDominantHandChanged();

	void UpdateCachePlayerName();
	void UpdateCachePlayerDominantHand();
public:	
	UARVIIntegrationSubSystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	* Indicates if the module Messages has been initialized
	* @return 
	*/
	bool GetMessagesWasInitialize() const {
		return bMessagesWasInitialize;
	}

	/**
	* Indicates if the module SessionVariables has been initialized
	* @return
	*/
	bool GetSessionVariablesWasInitialize() const {
		return bSessionVariablesWasInitialize;
	}

	/**
	 * Gets the SDK version
	 * @return	version string representation
	 */
	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	FString GetSDKVersion() const;

	/**
	 * Will return a MessagesSystem initialization error
	 * @return	string error
	 */
	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	FString GetMessagesError() const;

	/**
	 * Will return a SessionVariablesSystem initialization error
	 * @return	string error
	 */
	UFUNCTION(BlueprintCallable, Category = "ARVI Integration")
	FString GetSessionVariablesError() const;

	/**
	 * Checks application entitlement
	 * @param appKey		Application Key, provided with API
	 * @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	 * @param OnFailed		The handler that is called when an asynchronous operation fails
	 * @return				True if the asynchronous operation has started and one of the handlers will be called later
	 */
	bool IsApplicationEntitled(FString AppKey, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Checks application entitlement (Deprecated)
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool IsApplicationEntitledOld(FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);
	
	/**
	* Notifies platform about starting game server
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool ServerStarted(FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Notifies platform about completing the game
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool GameCompleted(FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Notifies platform about operator calling
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool CallOperator(FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sets an audio chat channel
	* @param ChatChannel	TeamID	Channel number. Acceptable values are TEAM01..TEAM10. Set ROOT, if you want join to default channel
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool SetAudioChatChannel(EAudioChatChannel ChatChannel, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sends text message to platform
	* @param Message		Message text
	* @param Group			Optional parameter. The name of the group by which the messages will be grouped
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 2Kb
	*/
	bool SendGameMessage(const FString& Message, const FName& Group = NAME_None, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sends service/debug message which will be logged on platform
	* @param Message		Message text
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 10Kb
	*/
	bool SendLogMessage(const FString& Message, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sends warning message which will be shown in administrative panel
	* @param Message		Message text
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 1 time per second and 10 times per minute. Message length should not exceed 2Kb
	*/
	bool SendWarningMessage(const FString& Message, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sends tracking message which will be tracked on log timelin
	* @param Message		Message text
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 1Kb
	*/
	bool SendTrackingMessage(const FString& Message, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Activates in-game command. If several commands have the same activation message, then they will all be activated
	* @param ActivationMessage		Activation message
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 128
	*/
	bool ActivateInGameCommand(const FString& ActivationMessage, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Activates multiple in-game commands at once. If several commands have the same activation message, then they will all be activated
	* @param ActivationMessages		Activation messages
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 2048
	*/
	bool ActivateInGameCommands(const TArray<FString>& ActivationMessages, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Deactivates in-game command. If several commands have the same deactivation message, then they will all be deactivated
	* @param DeactivationMessage	Deactivation message
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 128
	*/
	bool DeactivateInGameCommand(const FString& DeactivationMessage, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Deactivates multiple in-game commands at once. If several commands have the same deactivation message, then they will all be deactivated
	* @param DeactivationMessage	Deactivation message
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 2048
	*/
	bool DeactivateInGameCommands(const TArray<FString>& DeactivationMessages, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sets session-related raw data by name
	* @param Name					Data name
	* @param Data					Binary data to save
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: Data size should not exceed 10Mb
	*/
	bool SetSessionData(const FString& Name, const TArray<uint8>& Data, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sets session-related serialized data by name
	* @param Name					Data name
	* @param Value					Value to serialize
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: Data size should not exceed 10Mb
	*/
	template<typename T>
	bool SetSessionData(const FString& Name, T Value, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr) {
		TArray<uint8> Buff;
		FMemoryWritingAssistant Writer(Buff);

		Writer << Value;

		if (Writer.IsError())
			return false;

		return SetSessionData(Name, Buff, OnCompleted, OnFailed);
	}

	/**
	* Gets session-related raw data by name
	* @param Name					Data name
	* @param Data					Returned binary data
	* @return						True if the data by name was found
	*/
	bool TryGetSessionData(const FString& Name, TArray<uint8>& Data);

	/**
	* Gets session-related serialized data by name
	* @param Name					Data name
	* @param Value					Serialized value
	* @return						True if the data by name could be found and loaded
	*/
	template<typename T>
	bool TryGetSessionData(const FString& Name, T& Value) {
		TArray<uint8> Buff;
		if (!TryGetSessionData(Name, Buff))
			return false;
		FMemoryReadingAssistant Reader(Buff);
		Reader << Value;
		return !Reader.IsError();
	}

	/**
	* Gets UI settings data by name
	* @param Name					Data name
	* @param Data					Returned binary data
	* @return						True if the data by name was found
	*/
	bool TryGetUISettingsData(const FString& Name, FString& Value);

	/**
	* Gets the number of players in the session
	* @return	Number of players in the session
	*/
	UFUNCTION(BlueprintGetter)
	int GetPlayersCount() const {
		return PlayersCount;
	};

	/** 
	* Gets game server IP address
	* @return	Game server IP address
	*/
	UFUNCTION(BlueprintGetter)
	const FString& GetServerIP() const {
		return ServerIP;
	};

	/**
	* Gets game session language
	* @return	Game session language
	*/
	UFUNCTION(BlueprintGetter)
	const FString& GetSessionLanguage() const {
		return SessionLanguage;
	}

	/**
	* Gets game session time in seconds
	* @return	Game session time in seconds
	*/
	UFUNCTION(BlueprintGetter)
	int GetSessionTime() const {
		return SessionTime;
	}

	/**
	 * Gets a unique game session identifier
	 * @return	Unique game session identifier
	 */
	UFUNCTION(BlueprintGetter)
	const FString& GetSessionID() const {
		return SessionID;
	}

	/**
	* Gets player's ID
	* @return	Player's ID
	*/
	UFUNCTION(BlueprintGetter)
	const FString& GetPlayerID() const {
		return PlayerID;
	}

	/**
	* Gets the name of the player
	* @return Player's name
	*/
	UFUNCTION(BlueprintGetter)
	const FString& GetPlayerName() const {
		return PlayerName;
	}

	/**
	* Gets the dominant hand of the player
	* @return Player's dominant hand
	*/
	UFUNCTION(BlueprintGetter)
	EPlayerDominantHand GetPlayerDominantHand() const {
		return PlayerDominantHand;
	}

	/**
	* Gets if the game should track headset cord twisting
	* @return Return true if the game should track headset cord twisting, false otherwise
	*/
	UFUNCTION(BlueprintGetter)
	bool GetShouldApplicationTrackCordTwisting() const {
		return bShouldApplicationTrackCordTwisting;
	}

	/**
	* Gets if the application is running in trial mode
	* @return Return true if the application is in trial mode, false otherwise
	*/
	UFUNCTION(BlueprintGetter)
	bool GetIsApplicationInTrialMode() const {
		return bIsApplicationInTrialMode;
	}

	/**
	* Sets the new player name
	* @param NewPlayerName			New player's name
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: Name length should not exceed 15
	*/
	bool SetPlayerName(const FString& NewPlayerName, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);

	/**
	* Sets the new player dominant hand
	* @param NewPlayerDominantHand		New value for the player's dominant hand
	* @param OnCompleted				The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed					The handler that is called when an asynchronous operation fails
	* @return							True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool SetPlayerDominantHand(EPlayerDominantHand NewPlayerDominantHand, FOnARVIIntegrationRequestCompleted OnCompleted = nullptr, FOnARVIIntegrationRequestFailed OnFailed = nullptr);
};
