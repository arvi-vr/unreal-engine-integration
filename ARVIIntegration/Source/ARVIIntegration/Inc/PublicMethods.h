// Copyright © 2018-2022 ARVI VR Inc.
#pragma once

#include "PublicTypes.h"

#ifdef LIBARVI_INTERNAL
#define LIBARVI_PUBLIC __declspec(dllexport)
#elif LIBARVI_STATIC
#define LIBARVI_PUBLIC
#else
#define LIBARVI_PUBLIC __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	/**
	* Gets the SDK version
	* @return	Pointer to version string
	*/
	LIBARVI_PUBLIC const wchar_t* GetSDKVersion();
	LIBARVI_PUBLIC int Responses_Get(RESPONSE_ID* Responses, int* ResponseCount);
	LIBARVI_PUBLIC REQUEST_TYPE Response_GetType(RESPONSE_ID ResponseID);
	LIBARVI_PUBLIC REQUEST_ID Response_GetRequestID(RESPONSE_ID ResponseID);
	LIBARVI_PUBLIC int Response_IsSuccess(RESPONSE_ID ResponseID);
	LIBARVI_PUBLIC int Response_GetErrorCode(RESPONSE_ID ResponseID);
	LIBARVI_PUBLIC const wchar_t* Response_GetErrorMessage(RESPONSE_ID ResponseID);
	LIBARVI_PUBLIC int Response_GetDataSize(RESPONSE_ID ResponseID);
	LIBARVI_PUBLIC int Response_GetData(RESPONSE_ID ResponseID, char* Buff, int* BuffSize);
	LIBARVI_PUBLIC void Response_Free(RESPONSE_ID ResponseID);
	
	LIBARVI_PUBLIC int Messages_Initialize();
	LIBARVI_PUBLIC int Messages_Initialized();
	LIBARVI_PUBLIC void Messages_Finalize();
	LIBARVI_PUBLIC const wchar_t* Messages_GetErrorMessage();
	LIBARVI_PUBLIC int Messages_Get(MESSAGE_ID* Messages, int* MessageCount);
	LIBARVI_PUBLIC const wchar_t* Message_GetMethod(MESSAGE_ID Message);
	LIBARVI_PUBLIC const wchar_t* Message_GetName(MESSAGE_ID Message);
	LIBARVI_PUBLIC int Message_GetParamsCount(MESSAGE_ID Message);
	LIBARVI_PUBLIC const wchar_t* Message_GetParamName(MESSAGE_ID Message, int index);
	LIBARVI_PUBLIC const wchar_t* Message_GetParamValue(MESSAGE_ID Message, int index);
	LIBARVI_PUBLIC int Message_GetDataSize(MESSAGE_ID Message);
	LIBARVI_PUBLIC int Message_GetData(MESSAGE_ID Message, char* Buff, int* BuffSize);
	LIBARVI_PUBLIC int Message_GetIsInternalMessage(MESSAGE_ID Message);
	LIBARVI_PUBLIC int Message_HandleInternalMessage(MESSAGE_ID Message, EVENT_LIST_PTR* Events);

	LIBARVI_PUBLIC int Message_SetResponse(MESSAGE_ID Message, const wchar_t* ContentType, int StatusCode, const wchar_t* StatusText, const char* Buff, int BuffSize);
	LIBARVI_PUBLIC void Message_Free(MESSAGE_ID Message);

	LIBARVI_PUBLIC EVENT_NOTIFICATION_TYPE Event_GetType(EVENT_ID Event);
	LIBARVI_PUBLIC int Event_GetPlayerName(EVENT_ID Event, wchar_t** PlayerName);
	LIBARVI_PUBLIC int Event_GetPlayerDominantHand(EVENT_ID Event, DOMINANT_HAND* PlayerDominantHand);

	LIBARVI_PUBLIC int EventList_GetCount(const EVENT_LIST_PTR Events);
	LIBARVI_PUBLIC EVENT_ID EventList_GetEvent(const EVENT_LIST_PTR Events, int Index);
	LIBARVI_PUBLIC void EventList_Free(EVENT_LIST_PTR Events);

	LIBARVI_PUBLIC int SessionVariables_Initialize();
	LIBARVI_PUBLIC int SessionVariables_Initialized();
	LIBARVI_PUBLIC void SessionVariables_Finalize();
	LIBARVI_PUBLIC const wchar_t* SessionVariables_GetErrorMessage();

	LIBARVI_PUBLIC void Requests_Initialize();
	LIBARVI_PUBLIC void Requests_Finalize(int WaitSavedRequestsBeProcessed);

	/**
	* Checks application entitlement
	* @param AppKey	Application Key, provided with API
	* @return		Associated Request ID
	*/
	REQUEST_ID LIBARVI_PUBLIC IsApplicationEntitled(const wchar_t* AppKey);

	/**
	* Checks application entitlement (Deprecated)
	* @return		Associated Request ID
	*/
	REQUEST_ID LIBARVI_PUBLIC IsApplicationEntitledOld();

	/**
	* Notifies platform about starting game serv
	* @return		Associated Request ID
	*/
	REQUEST_ID LIBARVI_PUBLIC ServerStarted();

	/**
	* Notifies platform about completing the game
	* @return		Associated Request ID
	*/
	REQUEST_ID LIBARVI_PUBLIC GameCompleted();

	/**
	* Notifies platform about operator calling
	* @return		Associated Request ID 
	*/
	REQUEST_ID LIBARVI_PUBLIC CallOperator();

	/**
	* Sets an audio chat channel
	* @param TeamID	Channel number. Acceptable values are TEAM_01..TEAM_10. Set TEAM_PUBLIC, if you want join to default channel
	* @return		Associated Request ID
	*/
	REQUEST_ID LIBARVI_PUBLIC SetAudioChatChannel(TEAM_ID TeamID);

	/**
	* Sends text message to platform
	* @param Message	Message text
	* @param GroupName	Optional parameter. The name of the group by which the messages will be grouped
	* @return			Associated Request ID
	* @note				Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 2Kb
	*/
	REQUEST_ID LIBARVI_PUBLIC SendGameMessage(const wchar_t* Message, const wchar_t* GroupName);

	/**
	* Sends tracking message which will be tracked on log timeline
	* @param Message	Message text
	* @return			Associated Request ID
	* @note				Limitations: no more than 10 time per second and 100 times per minute. Message length should not exceed 1Kb
	*/
	REQUEST_ID LIBARVI_PUBLIC SendTrackingMessage(const wchar_t* Message);

	/**
	* Sends service/debug message which will be logged on platform
	* @param Message	Message text
	* @return			Associated Request ID
	* @note				Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 10Kb
	*/
	REQUEST_ID LIBARVI_PUBLIC SendLogMessage(const wchar_t* Message);

	/**
	* Sends warning message which will be shown in administrative panel
	* @param Message	Message text
	* @return			Associated Request ID
	* @note				Limitations: no more than 1 time per second and 10 times per minute. Message length should not exceed 2Kb
	*/
	REQUEST_ID LIBARVI_PUBLIC SendWarningMessage(const wchar_t* Message);

	/**
	* Activates in-game command. If several commands have the same activation message, then they will all be activated
	* @param ActivationMessage	Activation message
	* @return					Associated Request ID
	* @note						Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 128
	*/
	REQUEST_ID LIBARVI_PUBLIC ActivateInGameCommand(const wchar_t* ActivationMessage);

	/**
	* Deactivates in-game command. If several commands have the same deactivation message, then they will all be deactivated
	* @param DeactivationMessage	Deactivation message
	* @return						Associated Request ID
	* @note							Limitations: no more than 10 times per second and 100 times per minute. Message length should not exceed 128
	*/
	REQUEST_ID LIBARVI_PUBLIC DeactivateInGameCommand(const wchar_t* DeactivationMessage);


	/**
	* Sets session-related data by name
	* @param Name					Data name
	* @param Buff					Binary data to save
	* @param BuffSize				Count of bytes in Buff
	* @return						Associated Request ID
	* @note							Limitations: Data size should not exceed 10Mb
	*/
	LIBARVI_PUBLIC REQUEST_ID SetSessionData(const wchar_t* Name, const char* Buff, int BuffSize);

	/**
	* Gets session-related data by name
	* @param Name					Data name
	* @param Buff					Buffer for writing data
	* @param BuffSize				Count of bytes in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetSessionData(const wchar_t* Name, char* Buff, int* BuffSize);
	
	/**
	* Gets the underlying session-related data by name
	* @param Name					Data name
	* @param Buff					Buffer for writing data
	* @param BuffSize				Count of bytes in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetLaunchData(const wchar_t* Name, wchar_t* Buff, int* BuffCharCount);

	/**
	* Gets UI settings data by name
	* @param Name					Data name
	* @param Buff					Buffer for writing data
	* @param BuffSize				Count of bytes in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetUISettingsData(const wchar_t* Name, wchar_t* Buff, int* BuffCharCount);

	/**
	* Gets the number of players in the session
	* @param Value					Number of players in the session
	* @return						Non-zero value if the success
	*/
	LIBARVI_PUBLIC int TryGetPlayersCount(int* Value);

	/**
	* Gets game server IP address
	* @param Buff					Buffer for writing game server IP address
	* @param BuffCharCount			Count of chars in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetServerIP(wchar_t* Buff, int* BuffCharCount);

	/**
	* Gets game session language
	* @param Buff					Buffer for writing game session language
	* @param BuffCharCount			Count of chars in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetSessionLanguage(wchar_t* Buff, int* BuffCharCount);

	/**
	* Gets game session time in seconds
	* @param Value					Game session time in seconds
	* @return						Non-zero value if the success
	*/
	LIBARVI_PUBLIC int TryGetSessionTime(int* Value);

	/**
	* Gets a unique game session identifier
	* @param Buff					Buffer for writing unique game session identifier
	* @param BuffCharCount			Count of chars in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetSessionID(wchar_t* Buff, int* BuffCharCount);


	/**
	* Gets player's ID
	* @param Buff					Buffer for writing player's ID
	* @param BuffCharCount			Count of chars in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetPlayerID(wchar_t* Buff, int* BuffCharCount);

	/**
	* Gets the name of the player
	* @param Buff					Buffer for writing name of the player
	* @param BuffCharCount			Count of chars in Buff
	* @return						Non-zero value if the data is written
	*/
	LIBARVI_PUBLIC int TryGetPlayerName(wchar_t* Buff, int* BuffCharCount);

	/**
	* Gets the dominant hand of the player
	* @param Value					Dominant hand of the player
	* @return						Non-zero value if the success
	*/
	LIBARVI_PUBLIC int TryGetPlayerDominantHand(DOMINANT_HAND* Value);

	/**
	* Sets the new player name
	* @param PlayerName				New player's name
	* @param Changed				Will contain a non-null value if the name has been changed
	* @return						Associated Request ID
	* @note							Limitations: Name length should not exceed 15
	*/
	LIBARVI_PUBLIC REQUEST_ID SetPlayerName(const wchar_t* PlayerName, int* Changed);

	/**
	* Sets the new player dominant hand
	* @param PlayerDominantHand		New value for the player's dominant hand
	* @param Changed				Will contain a non-null value if the hand has been changed
	* @return						Associated Request ID
	*/
	LIBARVI_PUBLIC REQUEST_ID SetPlayerDominantHand(DOMINANT_HAND PlayerDominantHand, int* Changed);


	/**
	* Tries to get the value of the hand by its string representation
	* @param RawValue				Player dominant hand string representation
	* @param PlayerDominantHand		Player dominant hand
	* @return						True in case of success call
	*/
	LIBARVI_PUBLIC int TryGetPlayerDominantHandFromString(const wchar_t* RawValue, DOMINANT_HAND* PlayerDominantHand);

	/**
	* Checks if the application should track cord twisting
	* @return		True in case of application should track cord twisting
	*/
	LIBARVI_PUBLIC int GetShouldApplicationTrackCordTwisting();

	/**
	* Checks if the application is running in trial mode
	* @return		True in case of application is running in trial mode
	*/
	LIBARVI_PUBLIC int GetIsApplicationInTrialMode();

#ifdef __cplusplus
}
#endif