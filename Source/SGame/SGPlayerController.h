// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Messaging.h"
#include "SGameMessages.h"

#include "SGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SGAME_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASGPlayerController(const FObjectInitializer& ObjectInitializer);
	
private:
	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;
};
