// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGGlobalGameInstance.h"


USGGlobalGameInstance::USGGlobalGameInstance(const FObjectInitializer& ObjectInitializer)
{
	MessageEndpoint = FMessageEndpoint::Builder("GlobalGameInstance");
}

void USGGlobalGameInstance::StartGame()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message 		
		MessageEndpoint->Publish(new FMessage_Gameplay_GameStart(), EMessageScope::Process
		);
	}

	// Start the new round
	NewRound();
}

void USGGlobalGameInstance::NewRound()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_RondBegin;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void USGGlobalGameInstance::ForceCollect()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message
		FMessage_Gameplay_CollectLinkLine* CollectLinkLineMessage = new FMessage_Gameplay_CollectLinkLine();
		MessageEndpoint->Publish(CollectLinkLineMessage, EMessageScope::Process);
	}
}

void USGGlobalGameInstance::PlayerEndBuildPath()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerEndBuildPath;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}
