// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_ARVIIntegration.h"
#include "ARVIIntegrationRequest.h"

#define ARVIINTERAGTION_IMPLEMENTATION(Name) \
UK2Node_##Name::UK2Node_##Name(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {\
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UARVIIntegrationRequest_##Name, Name); \
	ProxyFactoryClass = UARVIIntegrationRequest_##Name::StaticClass(); \
	ProxyClass = UARVIIntegrationRequest_##Name::StaticClass(); \
	ProxyActivateFunctionName = GET_FUNCTION_NAME_CHECKED(UARVIIntegrationRequest_##Name, Execute); \
}

ARVIINTERAGTION_IMPLEMENTATION(IsApplicationEntitled)
ARVIINTERAGTION_IMPLEMENTATION(ServerStarted)
ARVIINTERAGTION_IMPLEMENTATION(GameCompleted)
ARVIINTERAGTION_IMPLEMENTATION(CallOperator)
ARVIINTERAGTION_IMPLEMENTATION(SetAudioChatChannel)
ARVIINTERAGTION_IMPLEMENTATION(SendGameMessage)
ARVIINTERAGTION_IMPLEMENTATION(SendLogMessage)
ARVIINTERAGTION_IMPLEMENTATION(SendWarningMessage)
ARVIINTERAGTION_IMPLEMENTATION(SendTrackingMessage)

ARVIINTERAGTION_IMPLEMENTATION(ActivateInGameCommand)
ARVIINTERAGTION_IMPLEMENTATION(DeactivateInGameCommand)

#undef ARVIINTERAGTION_IMPLEMENTATION