// Copyright © 2018-2022 ARVI VR Inc.


#include "ARVIIntegrationSubSystem.h"
#include "ARVIIntegrationLog.h"
#include "ARVIIntegrationLibrary.h"
#include "PublicMethods.h"

#include "Misc/CommandLine.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

static TEAM_ID ConvertChatChannel(EAudioChatChannel ChatChannel) {
	switch (ChatChannel) {
	case EAudioChatChannel::ACC_Public:
		return TEAM_ID::TEAM_PUBLIC;
	case EAudioChatChannel::ACC_Team01:
		return TEAM_ID::TEAM_1;
	case EAudioChatChannel::ACC_Team02:
		return TEAM_ID::TEAM_2;
	case EAudioChatChannel::ACC_Team03:
		return TEAM_ID::TEAM_3;
	case EAudioChatChannel::ACC_Team04:
		return TEAM_ID::TEAM_4;
	case EAudioChatChannel::ACC_Team05:
		return TEAM_ID::TEAM_5;
	case EAudioChatChannel::ACC_Team06:
		return TEAM_ID::TEAM_6;
	case EAudioChatChannel::ACC_Team07:
		return TEAM_ID::TEAM_7;
	case EAudioChatChannel::ACC_Team08:
		return TEAM_ID::TEAM_8;
	case EAudioChatChannel::ACC_Team09:
		return TEAM_ID::TEAM_9;
	case EAudioChatChannel::ACC_Team10:
		return TEAM_ID::TEAM_10;
	default:
		return TEAM_ID::TEAM_PUBLIC;
	}
}

static DOMINANT_HAND ConvertDominantHand(EPlayerDominantHand DominantHand) {
	switch (DominantHand)
	{
	case EPlayerDominantHand::PDH_Any:
		return DOMINANT_HAND::DOMINANT_HAND_ANY;
	case EPlayerDominantHand::PDH_Left:
		return DOMINANT_HAND::DOMINANT_HAND_LEFT;
	case EPlayerDominantHand::PDH_NotSet:
		return DOMINANT_HAND::DOMINANT_HAND_NOT_SET;
	case EPlayerDominantHand::PDH_Right:
		return DOMINANT_HAND::DOMINANT_HAND_RIGHT;
	default:
		return DOMINANT_HAND::DOMINANT_HAND_NOT_SET;
	}
}

static EPlayerDominantHand ConvertDominantHand(DOMINANT_HAND SDKDominantHand) {
	switch (SDKDominantHand)
	{
	case DOMINANT_HAND::DOMINANT_HAND_ANY:
		return EPlayerDominantHand::PDH_Any;
	case DOMINANT_HAND::DOMINANT_HAND_LEFT:
		return EPlayerDominantHand::PDH_Left;
	case DOMINANT_HAND::DOMINANT_HAND_NOT_SET:
		return EPlayerDominantHand::PDH_NotSet;
	case DOMINANT_HAND::DOMINANT_HAND_RIGHT:
		return EPlayerDominantHand::PDH_Right;
	default:
		return EPlayerDominantHand::PDH_NotSet;
	}
}
template <typename Func>
static bool TryGetStringData(const Func TryGetFunc, FString& Value) {
	TArray<wchar_t> Buff;
	int size;
	TryGetFunc(nullptr, &size);
	Buff.SetNum(size);
	size = Buff.Num();
	if (!TryGetFunc(Buff.GetData(), &size))
		return false;
	Buff.SetNum(size); 
	FWCharToTCHAR Converter(Buff.GetData(), Buff.Num());
	Value = FString(Converter.Length(), Converter.Get());
	return true;
}

void UARVIIntegrationSubSystem::RequestCompletedWorker()
{
	TArray<RESPONSE_ID> Responses;
	int Count = MaxResponsesPerCall;
	Responses.SetNum(Count);
	if (Responses_Get(Responses.GetData(), &Count)) {
		for (int index = 0; index < Count && index < Responses.Num(); ++index) {
			REQUEST_TYPE Type = Response_GetType(Responses[index]);
			REQUEST_ID ID = Response_GetRequestID(Responses[index]);
			int ErrorCode = Response_GetErrorCode(Responses[index]);
			FString ErrorMessage(WCHAR_TO_TCHAR(Response_GetErrorMessage(Responses[index])));
			FRequestCallback Callback;
			if (Requests.RemoveAndCopyValue((int64)ID, Callback)) {
				if (ErrorCode == ERROR_CODE::ERROR_NONE) {
					if (Callback.OnCompletedWithData.IsBound()) {
						if (Type != REQUEST_TYPE::REQUEST_DATA) {
							UE_LOG(LogARVIIntegration, Warning, TEXT("A response type with data is expected but this type is %d"), (int)Type);
							Callback.OnCompletedWithData.ExecuteIfBound(TArray<uint8>());
						}
						int Size = Response_GetDataSize(Responses[index]);
						TArray<uint8> Data;
						Data.SetNum(Size);
						Size = Data.Num();
						if (Response_GetData(Responses[index], (char*)Data.GetData(), &Size)) {
							Data.SetNum(Size);
							Callback.OnCompletedWithData.ExecuteIfBound(Data);
						}
						else{
							UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load response data"));
						}
					}
					Callback.OnCompleted.ExecuteIfBound();
				}
				else {
					Callback.OnFailed.ExecuteIfBound(ErrorCode, ErrorMessage);
				}
			}
			else {
				UE_LOG(LogARVIIntegration, Warning, TEXT("Unknown request ID"));
			}
		}
	}

}

void UARVIIntegrationSubSystem::MessageRequestWorker()
{
	TArray<MESSAGE_ID> Messages;
	int Count = MaxMessagesPerCall;
	Messages.SetNum(Count);
	if (Messages_Get(Messages.GetData(), &Count)) {
		Messages.SetNum(Count);
		for (int index = 0; index < Count && index < Messages.Num(); ++index) {
			if (Message_GetIsInternalMessage(Messages[index])) {
				EVENT_LIST_PTR Events = nullptr;
				if (Message_HandleInternalMessage(Messages[index], &Events)) {
					for (int EventIndex = 0; EventIndex < EventList_GetCount(Events); ++EventIndex) {
						EVENT_ID Event = EventList_GetEvent(Events, EventIndex);
						switch (EVENT_NOTIFICATION_TYPE EventType = Event_GetType(Event)) {
						case EVENT_NOTIFICATION_TYPE::EVENT_NOTIFICATION_PLAYER_DOMINANT_HAND_CHANGED:
							DispatchEventPlayerDominantHandChanged();
							break;
						case EVENT_NOTIFICATION_TYPE::EVENT_NOTIFICATION_PLAYER_NAME_CHANGED:
							DispatchEventPlayerNameChanged();
							break;
						default:
							UE_LOG(LogARVIIntegration, Warning, TEXT("Unknown event type %d"), (int)(EventType));
							break;
						}
					}			
					EventList_Free(Events);
				}
			}
			else {
				FARVIPlatformMessage PlatformMessage;
				PlatformMessage.Method = FString(WCHAR_TO_TCHAR(Message_GetMethod(Messages[index])));
				PlatformMessage.Name = FString(WCHAR_TO_TCHAR(Message_GetName(Messages[index])));
				int ParamsCount = Message_GetParamsCount(Messages[index]);
				TArray<FARVIPlatformMessageParam> Params;
				for (int ParamIndex = 0; ParamIndex < ParamsCount; ++ParamIndex) {
					FString ParamName(WCHAR_TO_TCHAR(Message_GetParamName(Messages[index], ParamIndex)));
					FString ParamValue(WCHAR_TO_TCHAR(Message_GetParamValue(Messages[index], ParamIndex)));
					PlatformMessage.Params.Add(FARVIPlatformMessageParam(ParamName, ParamValue));
				}
				int DataSize = Message_GetDataSize(Messages[index]);
				if (DataSize > 0) {
					PlatformMessage.Data.SetNum(DataSize);
					if (Message_GetData(Messages[index], (char*)PlatformMessage.Data.GetData(), &DataSize)) {
						if (DataSize > PlatformMessage.Data.Num())
							PlatformMessage.Data.SetNum(DataSize);
					}
					else {
						UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load message data"));
						PlatformMessage.Data.Empty();
					}
				}
				TArray<uint8> ResponseData;
				FString ContentType;
				if (DispatchPlatformMessage(PlatformMessage, ContentType, ResponseData)) {
					Message_SetResponse(Messages[index], TCHAR_TO_WCHAR(*ContentType), 200, L"OK", (const char*)ResponseData.GetData(), ResponseData.Num());
				}
			}
			Message_Free(Messages[index]);
		}
	}
}

static FVector SwapAxis(const FVector& Vector) {
	FVector Result;
	Result.X = Vector.Y;
	Result.Y = Vector.Z;
	Result.Z = Vector.X;
	return Result;
}

static TSharedPtr<FJsonObject> WriteToJson(const FVector& Vector) {
	TSharedPtr<FJsonObject> FieldObj = MakeShareable(new FJsonObject());
	FieldObj->SetNumberField("x", Vector.X);
	FieldObj->SetNumberField("y", Vector.Y);
	FieldObj->SetNumberField("z", Vector.Z);
	return FieldObj;
}

bool UARVIIntegrationSubSystem::DispatchPlayerPositionRequest(const FARVIPlatformMessage& Message, FString& ContentType, TArray<uint8>& Data)
{
	if (!PlayerPositionRequestHandler.IsBound())
		return false;
	FTransform PlayerPosition;
	if (!PlayerPositionRequestHandler.Execute(PlayerPosition))
		return false;
	FVector Location = SwapAxis(PlayerPosition.GetLocation() * 0.01f);
	FVector Forward = SwapAxis(PlayerPosition.GetRotation().GetForwardVector());
	FVector Up = SwapAxis(PlayerPosition.GetRotation().GetUpVector());

	Forward.Normalize();
	Up.Normalize();

	TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject());
	TSharedPtr<FJsonObject> PlayerObj = MakeShareable(new FJsonObject());
	TSharedPtr<FJsonObject> CameraObj = MakeShareable(new FJsonObject());
	Root->SetObjectField("player", PlayerObj);
	Root->SetObjectField("camera", CameraObj);
	PlayerObj->SetObjectField("position", WriteToJson(Location));
	PlayerObj->SetObjectField("forward", WriteToJson(Forward));
	PlayerObj->SetObjectField("up", WriteToJson(Up));
	CameraObj->SetObjectField("position", WriteToJson(Location));
	CameraObj->SetObjectField("forward", WriteToJson(Forward));
	CameraObj->SetObjectField("up", WriteToJson(Up));

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

	Data = UARVIIntegrationLibrary::ConvertPlatformMessageDataFromString(OutputString);

	ContentType = TEXT("application/json");
	return true;
}

bool UARVIIntegrationSubSystem::DispatchTimeLeftRequest(const FARVIPlatformMessage& Message, FString& ContentType, TArray<uint8>& Data)
{
	if (!TimeLeftRequestHandler.IsBound())
		return false;
	int TimeLeft = 0;
	if (!TimeLeftRequestHandler.Execute(TimeLeft))
		return false;
	FString TimeleftString = FString::FromInt(TimeLeft);
	Data = UARVIIntegrationLibrary::ConvertPlatformMessageDataFromString(TimeleftString);
	ContentType = TEXT("text/plain");
	return true;
}


bool UARVIIntegrationSubSystem::DispatchPlatformMessage(const FARVIPlatformMessage& Message, FString& ContentType, TArray<uint8>& Data)
{
	if (Message.Name == TEXT("position"))
		return DispatchPlayerPositionRequest(Message, ContentType, Data);
	else
		if (Message.Name == TEXT("TimeLeft"))
			return DispatchTimeLeftRequest(Message, ContentType, Data);
	if (PlatformMessageRequestHandler.IsBound())
		return PlatformMessageRequestHandler.Execute(Message, ContentType, Data);
	return false;
}

void UARVIIntegrationSubSystem::DispatchEventPlayerNameChanged()
{

	FString OldPlayerName = PlayerName;
	UpdateCachePlayerName();

	if (OldPlayerName.Compare(PlayerName) != 0)
		PlayerNameChangedHandler.Broadcast(PlayerName);
}

void UARVIIntegrationSubSystem::DispatchEventPlayerDominantHandChanged()
{
	EPlayerDominantHand OldPlayerDominantHand = PlayerDominantHand;
	UpdateCachePlayerDominantHand();
	if (OldPlayerDominantHand != PlayerDominantHand)
		PlayerDominantHandChangedHandler.Broadcast(PlayerDominantHand);
}

void UARVIIntegrationSubSystem::UpdateCachePlayerName()
{
	if (!::TryGetStringData(&::TryGetPlayerName, PlayerName)) {
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can't load player's name when updating"));
	}
}

void UARVIIntegrationSubSystem::UpdateCachePlayerDominantHand()
{
	DOMINANT_HAND RawHand;
	if (::TryGetPlayerDominantHand(&RawHand)) {
		PlayerDominantHand = ConvertDominantHand(RawHand);
		if (RawHand != ConvertDominantHand(PlayerDominantHand)) {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Unknown dominant hand value %d"), (int)(RawHand));
		}
	}
	else {
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can't load player's dominant hand when updating"));
	}
}

UARVIIntegrationSubSystem::UARVIIntegrationSubSystem(): Super()
{
	MaxResponsesPerCall = 1024;
	MaxMessagesPerCall = 1024;
	RequestTimeInterval = 0.1f;
	MessageTimeInterval = 0.1f;
}

void UARVIIntegrationSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	bool bSuccess = false;
	if (UGameInstance* GameInstance = GetGameInstance()) {
		GameInstance->GetTimerManager().SetTimer(RequestCompletedTimer, FTimerDelegate::CreateUObject(this, &ThisClass::RequestCompletedWorker), RequestTimeInterval, true);
		GameInstance->GetTimerManager().SetTimer(MessageRequestTimer, FTimerDelegate::CreateUObject(this, &ThisClass::MessageRequestWorker), RequestTimeInterval, true);
		bSuccess = true;
	}
	else {
		UE_LOG(LogARVIIntegration, Warning, TEXT("ARVI Inntgration SDK: %s, Can not initialize timer"), *(this->GetSDKVersion()));
	}
	
	if (Messages_Initialize()) {
		bMessagesWasInitialize = true;
	}
	else {
		bMessagesWasInitialize = false;
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can not initialize messages. ErrorMessage: %s"), Messages_GetErrorMessage());
	}
	Requests_Initialize();
	if (SessionVariables_Initialize()) {
		bSessionVariablesWasInitialize = true;
	}
	else {
		bSessionVariablesWasInitialize = false;
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can not initialize session variables. ErrorMessage: %s"), SessionVariables_GetErrorMessage());
	}

	if (bSessionVariablesWasInitialize) {
		bShouldApplicationTrackCordTwisting = !!::GetShouldApplicationTrackCordTwisting();
		bIsApplicationInTrialMode = !!::GetIsApplicationInTrialMode();
		if (!::TryGetPlayersCount(&PlayersCount)) {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load player's name"));
		}

		if (!TryGetStringData(&::TryGetServerIP, ServerIP)) {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load server ip"));
		}

		if (!TryGetStringData(&::TryGetSessionLanguage, SessionLanguage)) {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load session language"));
		}

		if (!::TryGetSessionTime(&SessionTime)) {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load session time"));
		}

		if (!TryGetStringData(&::TryGetSessionID, SessionID)) {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load session id"));
		}

		if (!TryGetStringData(&::TryGetPlayerID, PlayerID)) {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load player's id"));
		}

		UpdateCachePlayerName();
		UpdateCachePlayerDominantHand();
	}
	else {
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can not initialize session variables. Session parameters set by default"));
		PlayersCount = 1;
		ServerIP.Empty();
		SessionLanguage.Empty();
		SessionTime = 0;
		SessionID.Empty();
		PlayerID.Empty();
		PlayerName.Empty();
		PlayerDominantHand = EPlayerDominantHand::PDH_NotSet;
		bShouldApplicationTrackCordTwisting = true;
		bIsApplicationInTrialMode = false;
	}
	if (bSuccess && bMessagesWasInitialize && bSessionVariablesWasInitialize) {
		UE_LOG(LogARVIIntegration, Log, TEXT("ARVI Integration SDK: %s, Initializetion completed"), *(this->GetSDKVersion()));
	}
	else {
		UE_LOG(LogARVIIntegration, Log, TEXT("ARVI Integration SDK: %s, Initializetion failed"), *(this->GetSDKVersion()));
	}
}

void UARVIIntegrationSubSystem::Deinitialize()
{	
	if (UGameInstance* GameInstance = GetGameInstance()) {
		if (RequestCompletedTimer.IsValid()) {
			GameInstance->GetTimerManager().ClearTimer(RequestCompletedTimer);
			RequestCompletedTimer.Invalidate();
		}
		if (MessageRequestTimer.IsValid()) {
			GameInstance->GetTimerManager().ClearTimer(MessageRequestTimer);
			MessageRequestTimer.Invalidate();
		}
	}
	else {
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can not deinitialize"));
	}
	Messages_Finalize();
#if WITH_EDITOR
	Requests_Finalize(0);
#else
	Requests_Finalize(1);
#endif
	SessionVariables_Finalize();
	bMessagesWasInitialize = false;
	bSessionVariablesWasInitialize = false;
	Super::Deinitialize();
}

FString UARVIIntegrationSubSystem::GetSDKVersion() const
{
	return FString(WCHAR_TO_TCHAR(::GetSDKVersion()));
}

FString UARVIIntegrationSubSystem::GetMessagesError() const
{
	return FString(WCHAR_TO_TCHAR(::Messages_GetErrorMessage()));
}

FString UARVIIntegrationSubSystem::GetSessionVariablesError() const
{
	return FString(WCHAR_TO_TCHAR(::SessionVariables_GetErrorMessage()));
}

bool UARVIIntegrationSubSystem::IsApplicationEntitled(FString AppKey, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::IsApplicationEntitled(TCHAR_TO_WCHAR(*AppKey));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::IsApplicationEntitledOld(FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::IsApplicationEntitledOld();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::ServerStarted(FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::ServerStarted();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::GameCompleted(FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::GameCompleted();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::CallOperator(FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::CallOperator();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SetAudioChatChannel(EAudioChatChannel ChatChannel, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::SetAudioChatChannel(ConvertChatChannel(ChatChannel));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendGameMessage(const FString& Message, const FName& Group, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::SendGameMessage(TCHAR_TO_WCHAR(*Message), Group.IsNone() ? nullptr : TCHAR_TO_WCHAR(*Group.ToString()));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendLogMessage(const FString& Message, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::SendLogMessage(TCHAR_TO_WCHAR(*Message));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendWarningMessage(const FString& Message, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::SendWarningMessage(TCHAR_TO_WCHAR(*Message));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendTrackingMessage(const FString& Message, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::SendTrackingMessage(TCHAR_TO_WCHAR(*Message));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::ActivateInGameCommand(const FString& ActivationMessage, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::ActivateInGameCommand(TCHAR_TO_WCHAR(*ActivationMessage));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::DeactivateInGameCommand(const FString& DeactivationMessage, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::DeactivateInGameCommand(TCHAR_TO_WCHAR(*DeactivationMessage));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SetSessionData(const FString& Name, const TArray<uint8>& Data, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	REQUEST_ID ID = ::SetSessionData(TCHAR_TO_WCHAR(*Name), (const char*)Data.GetData(), Data.Num());
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::TryGetSessionData(const FString& Name, TArray<uint8>& Data)
{
	int size = 0;
	bool res = !!::TryGetSessionData(TCHAR_TO_WCHAR(*Name), nullptr, &size);
	if (!res && (size == 0))
		return false;
	Data.SetNum(size);
	size = Data.Num();
	res = !!::TryGetSessionData(TCHAR_TO_WCHAR(*Name), (char*)Data.GetData(), &size);
	if (res)
		Data.SetNum(size);
	else
		Data.SetNum(0);	
	return res;
}

bool UARVIIntegrationSubSystem::TryGetUISettingsData(const FString& Name, FString& Value)
{
	Value.Empty();
	int size = 0;
	bool res = !!::TryGetUISettingsData(TCHAR_TO_WCHAR(*Name), nullptr, &size);
	if (!res && (size == 0))
		return false;
	TArray<wchar_t> Buff;
	Buff.SetNum(size);
	size = Buff.Num();
	res = !!::TryGetUISettingsData(TCHAR_TO_WCHAR(*Name), Buff.GetData(), &size);
	if (res) {
		Buff.SetNum(size);
		FWCharToTCHAR Converter(Buff.GetData(), Buff.Num());
		Value = FString(Converter.Length(), Converter.Get());
	}
	return res;
}

bool UARVIIntegrationSubSystem::SetPlayerName(const FString& NewPlayerName, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	int Changed = 0;
	REQUEST_ID ID = ::SetPlayerName(TCHAR_TO_WCHAR(*NewPlayerName), &Changed);
	if (Changed != 0) {
		UpdateCachePlayerName();
		PlayerNameChangedHandler.Broadcast(PlayerName);
	}

	if (ID == INVALID_ID) {
		UE_LOG(LogARVIIntegration, Log, TEXT("Failed to create a request for notification about the change of player name"));
		return false;
	}
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SetPlayerDominantHand(EPlayerDominantHand NewPlayerDominantHand, FOnARVIIntegrationRequestCompleted OnCompleted, FOnARVIIntegrationRequestFailed OnFailed)
{
	int Changed = 0;
	REQUEST_ID ID = ::SetPlayerDominantHand(ConvertDominantHand(NewPlayerDominantHand), &Changed);

	if (Changed != 0) {
		UpdateCachePlayerDominantHand();
		PlayerDominantHandChangedHandler.Broadcast(PlayerDominantHand);
	}

	if (ID == INVALID_ID){
		UE_LOG(LogARVIIntegration, Log, TEXT("Failed to create a notification request for a player's dominant hand change"));
		return false;
	}
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}
