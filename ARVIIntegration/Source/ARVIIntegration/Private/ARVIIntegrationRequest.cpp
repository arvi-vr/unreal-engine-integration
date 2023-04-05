// Copyright © 2018-2022 ARVI VR Inc.


#include "ARVIIntegrationRequest.h"
#include "ARVIIntegrationSubSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "ARVIIntegrationLog.h"

void UARVIIntegrationRequest::HandleCompleted()
{
	OnCompleted.Broadcast(0, FString());
}

void UARVIIntegrationRequest::HandleFailed(int Code, FString Message)
{
	OnFailed.Broadcast(Code, Message);
}

bool UARVIIntegrationRequest::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return false;
}

void UARVIIntegrationRequest::Execute()
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull)) {
		if (UGameInstance* GameInstance = World->GetGameInstance()) {
			if (UARVIIntegrationSubSystem* Integration = GameInstance->GetSubsystem<UARVIIntegrationSubSystem>()) {
				if (!ExecuteInternal(Integration))
					HandleFailed(-1, "Can not create request");
			}
			else {
				UE_LOG(LogARVIIntegration, Warning, TEXT("Can not access to ARVIIntegrationSubSystem"));
				HandleFailed(-1, "Can not create request");
			}
		}
		else {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not access to GameInstance"));
			HandleFailed(-1, "Can not create request");
		}
	}
	else {
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can not access to World"));
		HandleFailed(-1, "Can not create request");
	}
}

bool UARVIIntegrationRequest_IsApplicationEntitled::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->IsApplicationEntitled(AppKey, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_IsApplicationEntitled::IsApplicationEntitled(UObject* WorldContextObject, FString appKey)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->AppKey = appKey;
	}
	return Request;
}

bool UARVIIntegrationRequest_ServerStarted::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->ServerStarted(FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_ServerStarted::ServerStarted(UObject* WorldContextObject)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
	}
	return Request;
}

bool UARVIIntegrationRequest_GameCompleted::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->GameCompleted(FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_GameCompleted::GameCompleted(UObject* WorldContextObject)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
	}
	return Request;
}

bool UARVIIntegrationRequest_CallOperator::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->CallOperator(FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_CallOperator::CallOperator(UObject* WorldContextObject)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
	}
	return Request;
}

bool UARVIIntegrationRequest_SetAudioChatChannel::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SetAudioChatChannel(ChatChannel, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SetAudioChatChannel::SetAudioChatChannel(UObject* WorldContextObject, EAudioChatChannel chatChannel)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->ChatChannel = chatChannel;
	}
	return Request;
}

bool UARVIIntegrationRequest_SendGameMessage::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SendGameMessage(Message, Group, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SendGameMessage::SendGameMessage(UObject* WorldContextObject, const FString& message, FName group)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Message = message;
		Request->Group = group;
	}
	return Request;
}

bool UARVIIntegrationRequest_SendLogMessage::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SendLogMessage(Message, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SendLogMessage::SendLogMessage(UObject* WorldContextObject, const FString& message)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Message = message;
	}
	return Request;
}

bool UARVIIntegrationRequest_SendWarningMessage::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SendWarningMessage(Message, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SendWarningMessage::SendWarningMessage(UObject* WorldContextObject, const FString& message)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Message = message;
	}
	return Request;
}

bool UARVIIntegrationRequest_SendTrackingMessage::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SendTrackingMessage(Message, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SendTrackingMessage::SendTrackingMessage(UObject* WorldContextObject,const FString& message)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Message = message;
	}
	return Request;
}

bool UARVIIntegrationRequest_ActivateInGameCommand::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->ActivateInGameCommand(Message, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_ActivateInGameCommand::ActivateInGameCommand(UObject* WorldContextObject, const FString& ActivationMessage)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Message = ActivationMessage;
	}
	return Request;
}

bool UARVIIntegrationRequest_DeactivateInGameCommand::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->DeactivateInGameCommand(Message, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_DeactivateInGameCommand::DeactivateInGameCommand(UObject* WorldContextObject, const FString& DeactivationMessage)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Message = DeactivationMessage;
	}
	return Request;
}

bool UARVIIntegrationRequest_SetSessionData::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SetSessionData(Name, Data, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SetSessionData::SetSessionData(UObject* WorldContextObject, const FString& Name, const TArray<uint8>& Data)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Name = Name;
		Request->Data = Data;
	}
	return Request;
}

void UARVIIntegrationRequestWithData::HandleCompleted(const TArray<uint8>& Data)
{
	OnCompleted.Broadcast(Data, 0, FString());
}

void UARVIIntegrationRequestWithData::HandleFailed(int Code, FString Message)
{
	OnFailed.Broadcast(TArray<uint8>(), Code, Message);
}

bool UARVIIntegrationRequestWithData::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return false;
}

void UARVIIntegrationRequestWithData::Execute()
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull)) {
		if (UGameInstance* GameInstance = World->GetGameInstance()) {
			if (UARVIIntegrationSubSystem* Integration = GameInstance->GetSubsystem<UARVIIntegrationSubSystem>()) {
				if (!ExecuteInternal(Integration))
					HandleFailed(-1, "Can not create request");
			}
			else {
				UE_LOG(LogARVIIntegration, Warning, TEXT("Can not access to ARVIIntegrationSubSystem"));
				HandleFailed(-1, "Can not create request");
			}
		}
		else {
			UE_LOG(LogARVIIntegration, Warning, TEXT("Can not access to GameInstance"));
			HandleFailed(-1, "Can not create request");
		}
	}
	else {
		UE_LOG(LogARVIIntegration, Warning, TEXT("Can not access to World"));
		HandleFailed(-1, "Can not create request");
	}
}

bool UARVIIntegrationRequest_SetPlayerName::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SetPlayerName(PlayerName, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SetPlayerName::SetPlayerName(UObject* WorldContextObject, const FString& NewPlayerName)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->PlayerName = NewPlayerName;
	}
	return Request;
}

bool UARVIIntegrationRequest_SetPlayerDominantHand::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->SetPlayerDominantHand(PlayerDominantHand, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_SetPlayerDominantHand::SetPlayerDominantHand(UObject* WorldContextObject, EPlayerDominantHand NewPlayerDominantHand)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->PlayerDominantHand = NewPlayerDominantHand;
	}
	return Request;
}

bool UARVIIntegrationRequest_ActivateInGameCommands::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->ActivateInGameCommands(Messages, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_ActivateInGameCommands::ActivateInGameCommands(UObject* WorldContextObject, const TArray<FString>& ActivationMessages)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Messages = ActivationMessages;
	}
	return Request;
}

bool UARVIIntegrationRequest_DeactivateInGameCommands::ExecuteInternal(UARVIIntegrationSubSystem* Integration)
{
	return Integration->DeactivateInGameCommands(Messages, FOnARVIIntegrationRequestCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequestFailed::CreateUObject(this, &ThisClass::HandleFailed));
}

UARVIIntegrationRequest* UARVIIntegrationRequest_DeactivateInGameCommands::DeactivateInGameCommands(UObject* WorldContextObject, const TArray<FString>& DeactivationMessages)
{
	ThisClass* Request = NewObject<ThisClass>();
	if (Request) {
		Request->WorldContext = WorldContextObject;
		Request->Messages = DeactivationMessages;
	}
	return Request;
}
