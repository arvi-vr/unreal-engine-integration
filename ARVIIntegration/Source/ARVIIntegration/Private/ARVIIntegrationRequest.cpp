// Fill out your copyright notice in the Description page of Project Settings.


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
	return Integration->IsApplicationEntitled(AppKey, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->ServerStarted(FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->GameCompleted(FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->CallOperator(FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->SetAudioChatChannel(ChatChannel, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->SendGameMessage(Message, Group, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->SendLogMessage(Message, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->SendWarningMessage(Message, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->SendTrackingMessage(Message, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->ActivateInGameCommand(Message, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
	return Integration->DeactivateInGameCommand(Message, FOnARVIIntegrationRequesCompleted::CreateUObject(this, &ThisClass::HandleCompleted), FOnARVIIntegrationRequesFailed::CreateUObject(this, &ThisClass::HandleFailed));
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
