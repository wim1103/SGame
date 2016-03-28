// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"

#include "SGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SGAME_API ASGGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASGGameMode(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts. */
	virtual void BeginPlay() override;

private:
	
};
