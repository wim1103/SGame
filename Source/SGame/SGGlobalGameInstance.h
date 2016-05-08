// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Messaging.h"

#include "FAsyncQueue.h"
#include "SGameMessages.h"
#include "SGGlobalGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SGAME_API USGGlobalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USGGlobalGameInstance();
	
	// Manually Start the game
	UFUNCTION(exec)
	void StartGame();

	// Manually Start the new round
	UFUNCTION(exec)
	void NewRound();

	// Force collect the link line
	UFUNCTION(exec)
	void ForceCollect();

	// Player finish builld the link line
	UFUNCTION(exec)
	void PlayerEndBuildPath();

	// Player finish builld the link line
	UFUNCTION(exec)
	void BeginAttack();

	// Test async queue
	UFUNCTION(exec)
	void TestAsyncQueue();
	void TestReplayAnimation()
	{
		UE_LOG(LogSGameAsyncTask, Log, TEXT("Play animation %d"), CurrentReplayIndex);
		CurrentReplayIndex++;
	}
	int CurrentReplayIndex;
	void FinishReplayAnimation()
	{
		UE_LOG(LogSGameAsyncTask, Log, TEXT("Finish play animation..."));
	}

	/** Get the timer instance from the actors world */
	class FTimerManager& GetWorldTimerManager() const;

private:
	
	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;

	// Async queue
	TSharedRef<FAsyncQueue, ESPMode::ThreadSafe> Queue;
};
