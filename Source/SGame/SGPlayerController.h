// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Messaging.h"
#include "SGameMessages.h"

#include "SGPlayerController.generated.h"

/**
 * SGame player controller
 */
UCLASS()
class SGAME_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASGPlayerController(const FObjectInitializer& ObjectInitializer);

	/** Event when play begins for this actor. */
	virtual void BeginPlay() override;
	
private:
	/** Player can input now*/
	void HandlePlayerBeginInput(const FMessage_Gameplay_PlayerBeginInput& Message, const IMessageContextRef& Context);

	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;
};
