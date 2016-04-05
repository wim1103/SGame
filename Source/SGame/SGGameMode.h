// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"
#include "SGLinkLine.h"
#include "SGGrid.h"

#include "SGGameMode.generated.h"

/** Types of tile base type. */
UENUM()
namespace ESGGameStatus
{
	enum Type
	{
		EGS_Init = 0,					// Initial value
		EGS_GameStart = 1,				// Shield
		ETT_Potion = 2,					// Potion
		ETT_Coin = 3,					// Coin
		ETT_Mana = 4,					// Mana
		ETT_Arrow = 5,					// Arrow
		ETT_Soldier = 6					// Soldier
	};
}

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
