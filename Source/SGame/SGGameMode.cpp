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
	UE_LOG(LogSGameProcedure, Log, TEXT("New round begin!"));
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
	UE_LOG(LogSGameProcedure, Log, TEXT("Player turn begin!"));

	// Reset the link line 
	checkSlow(CurrentLinkLine);
	CurrentLinkLine->ResetLinkState();

	// Reset the tile select info
	checkSlow(CurrentGrid);
	CurrentGrid->ResetTileSelectInfo();

	// Reset the tile link info
	CurrentGrid->ResetTileLinkInfo();

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
	UE_LOG(LogSGameProcedure, Log, TEXT("Player regenerate!"));

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
	UE_LOG(LogSGameProcedure, Log, TEXT("Player skill CD!"));

	// Change the next status to player begin input
	if (MessageEndpoint.IsValid())
	{
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerBeginInput;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void ASGGameMode::OnPlayerBeginInput()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Player begin input!"));

	// Tell the player, he begin input now
	if (MessageEndpoint.IsValid())
	{
		MessageEndpoint->Publish(new FMessage_Gameplay_PlayerBeginInput(), EMessageScope::Process);
	}

	// Reset the link line.
	checkSlow(CurrentLinkLine);
	CurrentLinkLine->ResetLinkState();
}

void ASGGameMode::OnPlayerEndBuildPath()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Player end build path!"));

	// First we need to forward to linkline for collect some tiles
	checkSlow(CurrentLinkLine);
	CurrentLinkLine->OnPlayerFinishBuildPath();

	// Currently, set a timer function to move to next stage
	// to give the falling animation time
	GetWorldTimerManager().SetTimer(PlayerEndInputTimer, this, &ASGGameMode::TimerPlayerEndInput, 5, false);
}

void ASGGameMode::TimerPlayerEndInput()
{
	// Change the next status to player end input
	if (MessageEndpoint.IsValid())
	{
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerEndInput;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
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
	UE_LOG(LogSGameProcedure, Log, TEXT("Game start!"));

	// Tell the grid to initialize the grid
	checkSlow(CurrentGrid);
	CurrentGrid->RefillGrid();

	// Then add the refilled tiles to the all tiles array
	
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
	case ESGGameStatus::EGS_PlayerBeginInput:
		OnPlayerBeginInput();
		break;
	case ESGGameStatus::EGS_PlayerEndBuildPath:
		OnPlayerEndBuildPath();
		break;
	case ESGGameStatus::EGS_PlayerEndInput:
		OnPlayerEndInput();
		break;
	case ESGGameStatus::EGS_EnemyAttack:
		OnEnemyAttack();
		break;
	case ESGGameStatus::EGS_RoundEnd:
		OnRoundEnd();
		break;
	case ESGGameStatus::EGS_GameOver:
		OnGameOver();
		break;
	default:
		UE_LOG(LogSGameProcedure, Error, TEXT("Unhandled game status!"));
		break;
	}
}

void ASGGameMode::OnEnemyAttack()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Enemy attack stage!"));

	checkSlow(MessageEndpoint.IsValid());

	// Enemy attack stage
	FMessage_Gameplay_EnemyBeginAttack* Message = new FMessage_Gameplay_EnemyBeginAttack();
	MessageEndpoint->Publish(Message, EMessageScope::Process);

	// Send next stage to round end
	FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
	GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_RoundEnd;
	MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
}

void ASGGameMode::OnRoundEnd()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Round end!"));

	checkSlow(MessageEndpoint.IsValid());

	// Check if game over
	if (CheckGameOver() == true)
	{
		// If then, send next state to game over
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_GameOver;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
	else
	{
		// If not, start a new round
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_RondBegin;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

bool ASGGameMode::CheckGameOver()
{
	return false;
}

void ASGGameMode::OnGameOver()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Game over!"));
}

void ASGGameMode::OnPlayerEndInput()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Player end input!"));

	checkSlow(MessageEndpoint.IsValid());

	// Do post player input thing

	// Send to enemy attack stage
	FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
	GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_EnemyAttack;
	MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
}
