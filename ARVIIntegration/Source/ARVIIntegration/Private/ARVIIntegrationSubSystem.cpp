// Fill out your copyright notice in the Description page of Project Settings.


#include "ARVIIntegrationSubSystem.h"
#include "ARVIIntegrationLog.h"
#include "ARVIIntegrationLibrary.h"
#include "PublicMethods.h"

#include "Misc/CommandLine.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

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
			FString ErrorMessage = Response_GetErrorMessage(Responses[index]);
			FRequestCallback Callback;
			if (Requests.RemoveAndCopyValue((int64)ID, Callback)) {
				if (ErrorCode == ERROR_CODE::ERROR_NONE) {
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
		for (int index = 0; index < Count && index < Messages.Num(); ++index) {
			FARVIPlatformMessage PlatformMessage;
			PlatformMessage.Method = Message_GetMethod(Messages[index]);
			PlatformMessage.Name = Message_GetName(Messages[index]);
			int ParamsCount = Message_GetParamsCount(Messages[index]);
			TArray<FARVIPlatformMessageParam> Params;
			for (int ParamIndex = 0; ParamIndex < ParamsCount; ++ParamIndex) {
				FString ParamName = Message_GetParamName(Messages[index], ParamIndex);
				FString ParamValue = Message_GetParamValue(Messages[index], ParamIndex);
				PlatformMessage.Params.Add(FARVIPlatformMessageParam(ParamName, ParamValue));
			}
			int DataSize = Message_GetDataSize(Messages[index]); 
			if (DataSize > 0){
				PlatformMessage.Data.SetNum(DataSize);
				if (Message_GetData(Messages[index], (char*)PlatformMessage.Data.GetData(), &DataSize)) {
					if (DataSize > PlatformMessage.Data.Num())
						PlatformMessage.Data.SetNum(DataSize);
				}				
				else{
					UE_LOG(LogARVIIntegration, Warning, TEXT("Can not load message data"));
					PlatformMessage.Data.Empty();
				}
			}
			TArray<uint8> ResponseData;
			FString ContentType;
			if (DispatchPlatformMessage(PlatformMessage, ContentType, ResponseData)) {
				Message_SetResponse(Messages[index], *ContentType, 200, TEXT("OK"), (const char*)ResponseData.GetData(), ResponseData.Num());
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
	if (!TimeLefRequestHandler.IsBound())
		return false;
	int TimeLeft = 0;
	if (!TimeLefRequestHandler.Execute(TimeLeft))
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
	bShouldApplicationTrackCordTwisting = !FParse::Param(FCommandLine::Get(), TEXT("DisableCordTwistTracking"));
	bIsApplicationInTrialMode = FParse::Param(FCommandLine::Get(), TEXT("trial"));
	if (bSuccess && bMessagesWasInitialize) {
		UE_LOG(LogARVIIntegration, Log, TEXT("ARVI Inntgration SDK: %s, Initializetion completed"), *(this->GetSDKVersion()));
	}
	else {
		UE_LOG(LogARVIIntegration, Log, TEXT("ARVI Inntgration SDK: %s, Initializetion failed"), *(this->GetSDKVersion()));
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
	Super::Deinitialize();
}

FString UARVIIntegrationSubSystem::GetSDKVersion() const
{
	return FString(::GetSDKVersion());
}

FString UARVIIntegrationSubSystem::GetMessagesError() const
{
	return FString(::Messages_GetErrorMessage());
}

bool UARVIIntegrationSubSystem::IsApplicationEntitled(FString AppKey, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::IsApplicationEntitled(*AppKey);
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::IsApplicationEntitledOld(FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::IsApplicationEntitledOld();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::ServerStarted(FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::ServerStarted();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::GameCompleted(FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::GameCompleted();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::CallOperator(FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::CallOperator();
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

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

bool UARVIIntegrationSubSystem::SetAudioChatChannel(EAudioChatChannel ChatChannel, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::SetAudioChatChannel(ConvertChatChannel(ChatChannel));
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendGameMessage(const FString& Message, const FName& Group, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::SendGameMessage(*Message, Group.IsNone() ? nullptr : *Group.ToString());
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendLogMessage(const FString& Message, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::SendLogMessage(*Message);
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendWarningMessage(const FString& Message, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::SendWarningMessage(*Message);
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::SendTrackingMessage(const FString& Message, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::SendTrackingMessage(*Message);
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::ActivateInGameCommand(const FString& ActivationMessage, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::ActivateInGameCommand(*ActivationMessage);
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}

bool UARVIIntegrationSubSystem::DeactivateInGameCommand(const FString& DeactivationMessage, FOnARVIIntegrationRequesCompleted OnCompleted, FOnARVIIntegrationRequesFailed OnFailed)
{
	REQUEST_ID ID = ::DeactivateInGameCommand(*DeactivationMessage);
	if (ID == INVALID_ID)
		return false;
	Requests.Add((int64)ID, FRequestCallback(OnCompleted, OnFailed));
	return true;
}
