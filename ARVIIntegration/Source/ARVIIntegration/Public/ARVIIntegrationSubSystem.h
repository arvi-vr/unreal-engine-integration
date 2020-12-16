// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioChatTypes.h"
#include "ARVIPlatformMessage.h"
#include "ARVIIntegrationSubSystem.generated.h"

DECLARE_DELEGATE(FOnARVIIntegrationRequesCompleted);
DECLARE_DELEGATE_TwoParams(FOnARVIIntegrationRequesFailed, int, FString);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FPlayerPositionRequestDelegate, FTransform&, PlayerTransform);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FTimeLefRequestDelegate, int&, TimeLeft);
DECLARE_DYNAMIC_DELEGATE_RetVal_ThreeParams(bool, FPlatformMessageRequestDelegate, const FARVIPlatformMessage&, Message, FString&, ContentType, TArray<uint8>&, Data);

USTRUCT()
struct ARVIINTEGRATION_API FRequestCallback {
	GENERATED_BODY()
	FOnARVIIntegrationRequesCompleted OnCompleted;
	FOnARVIIntegrationRequesFailed OnFailed;

	FRequestCallback() : OnCompleted(nullptr), OnFailed(nullptr) {
	
	}

	FRequestCallback(FOnARVIIntegrationRequesCompleted onCompleted, FOnARVIIntegrationRequesFailed onFailed) : OnCompleted(onCompleted), OnFailed(onFailed) {

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
	FTimeLefRequestDelegate TimeLefRequestHandler;

	/**
	* Arbitrary request handler	
	*/
	UPROPERTY(BlueprintReadWrite, Category = "ARVI Integration")
	FPlatformMessageRequestDelegate PlatformMessageRequestHandler;
protected:
	/**
	* Indicates if the module Messages has been initialized
	*/
	UPROPERTY(BlueprintReadOnly, Category = "ARVI Integration")
	bool bMessagesWasInitialize;

	/**
	* Gets if the game should track headset cord twisting
	*/
	UPROPERTY(BlueprintReadOnly, Category = "ARVI Integration")
	bool bShouldApplicationTrackCordTwisting;

	/**
	* Gets if the application is running in trial mode
	*/
	UPROPERTY(BlueprintReadOnly, Category = "ARVI Integration")
	bool bIsApplicationInTrialMode;

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
	* Gets if the game should track headset cord twisting
	* @return
	*/
	bool GetShouldApplicationTrackCordTwisting() const {
		return bShouldApplicationTrackCordTwisting;
	}

	/**
	* Gets if the application is running in trial mode
	* @return
	*/
	bool GetIsApplicationInTrialMode() const {
		return bIsApplicationInTrialMode;
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
	 * Checks application entitlement
	 * @param appKey		Application Key, provided with API
	 * @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	 * @param OnFailed		The handler that is called when an asynchronous operation fails
	 * @return				True if the asynchronous operation has started and one of the handlers will be called later
	 */
	bool IsApplicationEntitled(FString AppKey, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Checks application entitlement (Deprecated)
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool IsApplicationEntitledOld(FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);
	
	/**
	* Notifies platform about starting game server
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool ServerStarted(FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Notifies platform about completing the game
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool GameCompleted(FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Notifies platform about operator calling
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool CallOperator(FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Sets an audio chat channel
	* @param ChatChannel	TeamID	Channel number. Acceptable values are TEAM01..TEAM10. Set ROOT, if you want join to default channel
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	*/
	bool SetAudioChatChannel(EAudioChatChannel ChatChannel, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Sends text message to platform
	* @param Message		Message text
	* @param Group			Optional parameter. The name of the group by which the messages will be grouped
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 2Kb
	*/
	bool SendGameMessage(const FString& Message, const FName& Group = NAME_None, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Sends service/debug message which will be logged on platform
	* @param Message		Message text
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 10Kb
	*/
	bool SendLogMessage(const FString& Message, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Sends warning message which will be shown in administrative panel
	* @param Message		Message text
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 1 time per second and 10 times per minute. Message length should not exceed 2Kb
	*/
	bool SendWarningMessage(const FString& Message, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Sends tracking message which will be tracked on log timelin
	* @param Message		Message text
	* @param OnCompleted	The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed		The handler that is called when an asynchronous operation fails
	* @return				True if the asynchronous operation has started and one of the handlers will be called later
	* @note					Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 1Kb
	*/
	bool SendTrackingMessage(const FString& Message, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Activates in-game command. If several commands have the same activation message, then they will all be activated
	* @param ActivationMessage		Activation message
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 128
	*/
	bool ActivateInGameCommand(const FString& ActivationMessage, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);

	/**
	* Deactivates in-game command. If several commands have the same deactivation message, then they will all be deactivated
	* @param DeactivationMessage	Deactivation message
	* @param OnCompleted			The handler that is called when an asynchronous operation completes successfully.
	* @param OnFailed				The handler that is called when an asynchronous operation fails
	* @return						True if the asynchronous operation has started and one of the handlers will be called later
	* @note							Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 128
	*/
	bool DeactivateInGameCommand(const FString& DeactivationMessage, FOnARVIIntegrationRequesCompleted OnCompleted = nullptr, FOnARVIIntegrationRequesFailed OnFailed = nullptr);
};
