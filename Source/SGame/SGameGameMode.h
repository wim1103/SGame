// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SGAME_API ASGameGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ASGameGameMode(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts. */
	virtual void BeginPlay() override;

private:
	
};
