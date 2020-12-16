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
	int LIBARVI_PUBLIC Responses_Get(RESPONSE_ID* Responses, int* ResponseCount);
	REQUEST_TYPE LIBARVI_PUBLIC Response_GetType(RESPONSE_ID ResponseID);
	REQUEST_ID LIBARVI_PUBLIC Response_GetRequestID(RESPONSE_ID ResponseID);
	int LIBARVI_PUBLIC Response_IsSuccess(RESPONSE_ID ResponseID);
	int LIBARVI_PUBLIC Response_GetErrorCode(RESPONSE_ID ResponseID);
	LIBARVI_PUBLIC const wchar_t* Response_GetErrorMessage(RESPONSE_ID ResponseID);
	void LIBARVI_PUBLIC Response_Free(RESPONSE_ID ResponseID);
	
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

	int LIBARVI_PUBLIC Message_SetResponse(MESSAGE_ID Message, const wchar_t* ContentType, int StatusCode, const wchar_t* StatusText, const char* Buff, int BuffSize);

	void LIBARVI_PUBLIC Message_Free(MESSAGE_ID Message);

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
#ifdef __cplusplus
}
#endif