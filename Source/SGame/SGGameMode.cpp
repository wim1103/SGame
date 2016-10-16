// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGGameMode.h"
#include "SGPlayerController.h"
#include "SGEnemyTileBase.h"

ASGGameMode::ASGGameMode(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ASGPlayerController::StaticClass();
	CurrentRound = 0;
	MinimunLengthLinkLineRequired = 3;
	CurrentPlayerPawn = 0;
	bShouldReplayLinkAnimation = true;

	PlayerSkillManager = CreateDefaultSubobject<USGPlayerSkillManager>(TEXT("PlayerSkillManager"));
}

void ASGGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create messaeng end point for game mode
	MessageEndpoint = FMessageEndpoint::Builder("Gameplay_GameMode")
		.Handling<FMessage_Gameplay_GameStart>(this, &ASGGameMode::HandleGameStart)
		.Handling<FMessage_Gameplay_GameStatusUpdate>(this, &ASGGameMode::HandleGameStatusUpdate)
		.Handling<FMessage_Gameplay_AllTileFinishMove>(this, &ASGGameMode::HandleAllTileFinishMoving)
		.Handling<FMessage_Gameplay_EnemyBeginAttack>(this, &ASGGameMode::HandleBeginAttack)
		.Handling<FMessage_Gameplay_CollectLinkLine>(this, &ASGGameMode::HandleCollectLinkLine)
		.WithInbox();
	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the game mode needed messages
		MessageEndpoint->Subscribe<FMessage_Gameplay_GameStart>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_GameStatusUpdate>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_AllTileFinishMove>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_EnemyBeginAttack>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_CollectLinkLine>();
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

ESGGameStatus ASGGameMode::GetCurrentGameStatus()
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

void ASGGameMode::HandleCollectLinkLine(const FMessage_Gameplay_CollectLinkLine& Message, const IMessageContextRef& Context)
{
	checkSlow(CurrentLinkLine != nullptr);

	if (CurrentLinkLine->LinkLineTiles.Num() == 0)
	{
		UE_LOG(LogSGame, Warning, TEXT("No tiles in the link line, nothing to collect"));
		return;
	}

	// Post a tile disappear message
	FMessage_Gameplay_LinkedTilesCollect* DisappearMessage = new FMessage_Gameplay_LinkedTilesCollect();
	for (int i = 0; i < CurrentLinkLine->LinkLineTiles.Num(); i++)
	{
		checkSlow(CurrentLinkLine->LinkLineTiles[i]);
		DisappearMessage->TilesAddressToCollect.Push(CurrentLinkLine->LinkLineTiles[i]->GetGridAddress());
	}

	if (MessageEndpoint.IsValid() == true)
	{
		MessageEndpoint->Publish(DisappearMessage, EMessageScope::Process);
	}
}

bool ASGGameMode::CollectTileArray(TArray<ASGTileBase*> inTileArrayToCollect)
{
	// Collect resouce array, using the resource type as index
	TArray<float> SumupResource;
	SumupResource.AddZeroed(static_cast<int32>(ESGResourceType::ETT_MAX));

	// Array of tile address should be collected, used for condense the grid
	TArray<int32> CollectedTileAddressArray;

	// Iterate the link tiles, retrieve their resources
	for (int i = 0; i < inTileArrayToCollect.Num(); i++)
	{
		const ASGTileBase* Tile = inTileArrayToCollect[i];
		checkSlow(Tile);

		// Insert into the collect tile array
		CollectedTileAddressArray.Add(Tile->GetGridAddress());

		// Collecte the resouces
		TArray<FTileResourceUnit> TileResource = Tile->GetTileResource();
		for (int j = 0; j < TileResource.Num(); j++)
		{
			// Sumup the resource
			SumupResource[static_cast<int32>(TileResource[j].ResourceType)] += TileResource[j].ResourceAmount;
		}
	}

	if (SumupResource.Num() > 0)
	{
		FMessage_Gameplay_ResourceCollect* ResouceCollectMessage = new FMessage_Gameplay_ResourceCollect();
		ResouceCollectMessage->SummupResouces = SumupResource;

		checkSlow(MessageEndpoint.IsValid());
		MessageEndpoint->Publish(ResouceCollectMessage, EMessageScope::Process);
	}

	// Finally, sent the message indicate the tiles are collected
	if (CollectedTileAddressArray.Num() > 0)
	{
		FMessage_Gameplay_LinkedTilesCollect* Message = new FMessage_Gameplay_LinkedTilesCollect();
		Message->TilesAddressToCollect = CollectedTileAddressArray;
		if (MessageEndpoint.IsValid() == true)
		{
			MessageEndpoint->Publish(Message, EMessageScope::Process);
		}
	}

	return true;
}

void ASGGameMode::OnPlayerTurnBegin()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Player turn begin!"));

	// Reset the link line 
	checkSlow(CurrentLinkLine);
	CurrentLinkLine->ResetLinkState();

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

void ASGGameMode::OnPlayerBeginInputStage()
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

	// Reset the tiles
	checkSlow(CurrentGrid);
	CurrentGrid->ResetTiles();
}

void ASGGameMode::OnPlayerEndBuildPathStage()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Player end build path!"));

	// Check if the linkline meet requirement
	if (IsLinkLineValid() == false)
	{
		// If not, set back the stage to player input
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerBeginInput;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
		return;
	}

	// First we need to forward to linkline for collect some tiles
	checkSlow(CurrentLinkLine);
	CurrentLinkLine->OnPlayerFinishBuildPath();

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

bool ASGGameMode::IsLinkLineValid()
{
	checkSlow(CurrentLinkLine);
	if (CurrentLinkLine->LinkLineTiles.Num() >= MinimunLengthLinkLineRequired)
	{
		return true;
	}

	return false;
}

void ASGGameMode::HandleBeginAttack(const FMessage_Gameplay_EnemyBeginAttack& Message, const IMessageContextRef& Context)
{
	float ShiledDamage = 0;
	float DirectDamage = 0;
	checkSlow(CurrentGrid);
	if (CalculateEnemyDamageToPlayer(ShiledDamage, DirectDamage) == true)
	{
		UE_LOG(LogSGame, Log, TEXT("Enemy will cause %f shield damage, and %f direct damage "), ShiledDamage, DirectDamage);

		// Send player pawn take damage message
		FMessage_Gameplay_PlayerTakeDamage* PlayerTakeDamageMessage = new FMessage_Gameplay_PlayerTakeDamage{ 0 };
		PlayerTakeDamageMessage->ShiledDamage = ShiledDamage;
		PlayerTakeDamageMessage->DirectDamage = DirectDamage;

		checkSlow(MessageEndpoint.IsValid());
		MessageEndpoint->Publish(PlayerTakeDamageMessage, EMessageScope::Process);

		CurrentGrid->StartAttackFadeAnimation();
	}
}

bool ASGGameMode::CalculateEnemyDamageToPlayer(float& outDamageCanBeShield, float& outDamageDirectToHP)
{
	checkSlow(CurrentGrid);

	// Iterate the grid to find the enemy tiles
	TArray<FTileDamageInfo> EnemyCauseDamageInfoArray;
	const TArray<ASGTileBase*>& GridTiles = CurrentGrid->GetGridTiles();
	for (int i = 0; i < GridTiles.Num(); i++)
	{
		const ASGEnemyTileBase* EnemyTile = Cast<ASGEnemyTileBase>(GridTiles[i]);
		if (EnemyTile != nullptr)
		{
			EnemyCauseDamageInfoArray.Add(EnemyTile->Data.CauseDamageInfo);
		}
	}

	if (EnemyCauseDamageInfoArray.Num() == 0)
	{
		// We don't find enemy tiles, so return false, means that there is no pending attack
		return false;
	}

	// Iterate the damage info array, calculate the final
	for (int i = 0; i < EnemyCauseDamageInfoArray.Num(); i++)
	{
		outDamageCanBeShield += EnemyCauseDamageInfoArray[i].InitialDamage * (1 - EnemyCauseDamageInfoArray[i].PiercingArmorRatio);
		outDamageDirectToHP += EnemyCauseDamageInfoArray[i].InitialDamage * (EnemyCauseDamageInfoArray[i].PiercingArmorRatio);
	}

	return true;
}

bool ASGGameMode::CanLinkToLastTile(const ASGTileBase* inTestTile)
{
	checkSlow(inTestTile);
	checkSlow(CurrentLinkLine != nullptr);
	if (CurrentLinkLine->LinkLineTiles.Num() == 0)
	{
		// First tile, can always be linked
		return true;
	}

	// Check the current tile can be linked with the last tile
	const ASGTileBase* LastTile = CurrentLinkLine->LinkLineTiles.Last();
	checkSlow(LastTile != nullptr);

	// Currently only the neighbor tiles can be selected
	checkSlow(CurrentGrid);
	if (CurrentGrid->AreAddressesNeighbors(inTestTile->GetGridAddress(), LastTile->GetGridAddress()) == false)
	{
		return false;
	}

	// Same tile type can always link together
	if (LastTile->Data.TileType == inTestTile->Data.TileType)
	{
		return true;
	}

	// Enemy links 
	if ((LastTile->Abilities.bCanLinkEnemy == true && inTestTile->Abilities.bEnemyTile == true) ||
		(LastTile->Abilities.bEnemyTile == true && inTestTile->Abilities.bCanLinkEnemy == true))
	{
		return true;
	}

	return false;
}

ASGSkillBase* ASGGameMode::CreatePlayerSkilkByName(FString inSkillName)
{
	checkSlow(PlayerSkillManager);
	return PlayerSkillManager->CreateSkillByName(this, inSkillName);
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
		OnPlayerBeginInputStage();
		break;
	case ESGGameStatus::EGS_PlayerEndBuildPath:
		OnPlayerEndBuildPathStage();
		break;
	case ESGGameStatus::EGS_PlayerEndInput:
		OnPlayerEndInputStage();
		break;
	case ESGGameStatus::EGS_EnemyAttack:
		OnEnemyAttackStage();
		break;
	case ESGGameStatus::EGS_RoundEnd:
		OnRoundEndStage();
		break;
	case ESGGameStatus::EGS_GameOver:
		OnGameOver();
		break;
	default:
		UE_LOG(LogSGameProcedure, Error, TEXT("Unhandled game status!"));
		break;
	}
}

void ASGGameMode::HandleAllTileFinishMoving(const FMessage_Gameplay_AllTileFinishMove& Message, const IMessageContextRef& Context)
{
	if (CurrentGameGameStatus == ESGGameStatus::EGS_PlayerEndInput)
	{
		// Send to enemy attack stage
		checkSlow(MessageEndpoint.IsValid());
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_EnemyAttack;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void ASGGameMode::OnEnemyAttackStage()
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

void ASGGameMode::OnRoundEndStage()
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

void ASGGameMode::OnPlayerEndInputStage()
{
	UE_LOG(LogSGameProcedure, Log, TEXT("Player end input!"));


}
