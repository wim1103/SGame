// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGGameMode.h"
#include "SGPlayerController.h"

ASGGameMode::ASGGameMode(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ASGPlayerController::StaticClass();
}

void ASGGameMode::BeginPlay()
{
	Super::BeginPlay();

	MessageEndpoint = FMessageEndpoint::Builder("Gameplay_GameMode")
		.Handling<FMessage_Gameplay_GameStart>(this, &ASGGameMode::HandleGameStart)
		.Handling<FMessage_Gameplay_GameStatusUpdate>(this, &ASGGameMode::HandleGameStatusUpdate)
		.WithInbox();
	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the game mode needed messages
		MessageEndpoint->Subscribe<FMessage_Gameplay_GameStart>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_GameStatusUpdate>();
	}
}

void ASGGameMode::SetNextStatus(ESGGameStatus::Type inNewStatus)
{
	NextGameStatus = inNewStatus;
}

ESGGameStatus::Type ASGGameMode::GetCurrentGameStatus()
{
	return CurrentGameGameStatus;
}

void ASGGameMode::OnBeginRound()
{
	UE_LOG(LogSGame, Log, TEXT("New round begin!"));
}

void ASGGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Need to process the inbox to make sure the callbacks function work
	if (MessageEndpoint.IsValid())
	{
		MessageEndpoint->ProcessInbox();
	}
}

void ASGGameMode::HandleGameStart(const FMessage_Gameplay_GameStart& Message, const IMessageContextRef& Context)
{
	UE_LOG(LogSGame, Log, TEXT("Game start!"));
}

void ASGGameMode::HandleGameStatusUpdate(const FMessage_Gameplay_GameStatusUpdate& Message, const IMessageContextRef& Context)
{
	CurrentGameGameStatus = Message.NewGameStatus;
	
	switch (CurrentGameGameStatus)
	{
	case ESGGameStatus::EGS_RondBegin:
		OnBeginRound();
		break;
	default:
		UE_LOG(LogSGame, Warning, TEXT("Illegal game status update!"));
		break;
	}
}
