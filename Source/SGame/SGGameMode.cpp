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
		.Handling<FMessage_Gameplay_NewTilePicked>(this, &ASGGameMode::HandleNewTileIsPicked)
		.WithInbox();
	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the game mode needed messages
		MessageEndpoint->Subscribe<FMessage_Gameplay_GameStart>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_GameStatusUpdate>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_NewTilePicked>();
	}

	// Find the grid actor in the world
	CurrentGrid = nullptr;
	for (TActorIterator<ASGGrid> It(GetWorld()); It; ++It)
	{
		if (CurrentGrid == nullptr)
		{
			CurrentGrid = *It;
		}
		else
		{
			UE_LOG(LogSGame, Warning, TEXT("There is more than more grid object in the level!"));
		}
	}
	if (CurrentGrid == nullptr)
	{
		UE_LOG(LogSGame, Warning, TEXT("There is no grid object in the level!"));
	}

	// Find the link line actor in the world
	CurrentLinkLine = nullptr;
	for (TActorIterator<ASGLinkLine> It(GetWorld()); It; ++It)
	{
		if (CurrentLinkLine == nullptr)
		{
			CurrentLinkLine = *It;
		}
		else
		{
			UE_LOG(LogSGame, Warning, TEXT("There is more than more link line object in the level!"));
		}
	}
	if (CurrentLinkLine == nullptr)
	{
		UE_LOG(LogSGame, Warning, TEXT("There is no link line object in the level!"));
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
	CurrentRound++;

	// Change the next status to new round begin
	if (MessageEndpoint.IsValid())
	{
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerTurnBegin;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void ASGGameMode::OnPlayerTurnBegin()
{
	UE_LOG(LogSGame, Log, TEXT("Player turn begin!"));

	// Change the next status to player regenerate
	if (MessageEndpoint.IsValid())
	{
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerRegengerate;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void ASGGameMode::OnPlayerRegenerate()
{
	UE_LOG(LogSGame, Log, TEXT("Player regenerate!"));

	// Change the next status to player skill CD
	if (MessageEndpoint.IsValid())
	{
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerSkillCD;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void ASGGameMode::OnPlayerSkillCD()
{
	UE_LOG(LogSGame, Log, TEXT("Player skill CD!"));

	// Change the next status to player skill CD
	if (MessageEndpoint.IsValid())
	{
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerInput;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void ASGGameMode::OnPlayerInput()
{
	UE_LOG(LogSGame, Log, TEXT("Player input!"));

	// Tell the player, he begin input now
	if (MessageEndpoint.IsValid())
	{
		MessageEndpoint->Publish(new FMessage_Gameplay_PlayerBeginInput(), EMessageScope::Process);
	}

	// Reset the link line.
	if (CurrentLinkLine != nullptr)
	{
		CurrentLinkLine->ResetLinkState();
	}
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
	case ESGGameStatus::EGS_PlayerTurnBegin:
		OnPlayerTurnBegin();
		break;
	case ESGGameStatus::EGS_PlayerRegengerate:
		OnPlayerRegenerate();
		break;
	case ESGGameStatus::EGS_PlayerSkillCD:
		OnPlayerSkillCD();
		break;
	case ESGGameStatus::EGS_PlayerInput:
		OnPlayerInput();
		break;
	default:
		UE_LOG(LogSGame, Error, TEXT("Unhandled game status!"));
		break;
	}
}

void ASGGameMode::HandleNewTileIsPicked(const FMessage_Gameplay_NewTilePicked& Message, const IMessageContextRef& Context)
{
	UE_LOG(LogSGame, Log, TEXT("Player Build Path with TileID: %d"), Message.TileAddress);

	if (CurrentGrid == nullptr || CurrentLinkLine == nullptr)
	{
		// Grid or the link line object is invalid
		return;
	}

	// fixme by reiwang: turn off the game status control
// 	if (CurrentGameGameStatus != ESGGameStatus::EGS_PlayerInput)
// 	{
// 		// Not in the player input state, return
// 		return;
// 	}

	const ASGTileBase* CurrentTile = CurrentGrid->GetTileFromGridAddress(Message.TileAddress);
	if (CurrentTile == nullptr)
	{
		UE_LOG(LogSGame, Warning, TEXT("Cannot get tile from the tile adderss %d"), Message.TileAddress);
		return;
	}

	// Tell the link line to build the path
	if (CurrentLinkLine != nullptr)
	{
		CurrentLinkLine->BuildPath(CurrentTile);
	}
}
