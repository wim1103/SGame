// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGCheatManager.h"
#include "SGTileBase.h"
#include "SGEnemyTileBase.h"
#include "SGPlayerController.h"
#include "SGGrid.h"
#include "SGSpritePawn.h"

USGCheatManager::USGCheatManager() : Queue(FAsyncQueue::Create())
{
	MessageEndpoint = FMessageEndpoint::Builder("CheatManagerMessageEP");
	CurrentReplayIndex = 0;
}

void USGCheatManager::BeginAttack()
{
	// Find the link line actor in the world
	for (TActorIterator<ASGEnemyTileBase> It(GetWorld()); It; ++It)
	{
		ASGEnemyTileBase* Tile = *It;
		Tile->EnemyAttack();
	}
}

void USGCheatManager::UseSkill(int32 inSkillIndex)
{
	ASGPlayerController* MyPC = GetOuterASGPlayerController();

	checkSlow(MyPC->SkillsArray.IsValidIndex(inSkillIndex) && MyPC->SkillsArray[inSkillIndex]);
	MyPC->SkillsArray[inSkillIndex]->PlayerUseSkill();
}


void USGCheatManager::StartGame()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message 		
		MessageEndpoint->Publish(new FMessage_Gameplay_GameStart(), EMessageScope::Process);
	}

	// Start the new round
	NewRound();
}

void USGCheatManager::NewRound()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_RondBegin;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void USGCheatManager::ForceCollect()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message
		FMessage_Gameplay_CollectLinkLine* CollectLinkLineMessage = new FMessage_Gameplay_CollectLinkLine();
		MessageEndpoint->Publish(CollectLinkLineMessage, EMessageScope::Process);
	}
}

void USGCheatManager::PlayerEndBuildPath()
{
	if (MessageEndpoint.IsValid() == true)
	{
		// Test: Send game start message
		FMessage_Gameplay_GameStatusUpdate* GameStatusUpdateMesssage = new FMessage_Gameplay_GameStatusUpdate();
		GameStatusUpdateMesssage->NewGameStatus = ESGGameStatus::EGS_PlayerEndBuildPath;
		MessageEndpoint->Publish(GameStatusUpdateMesssage, EMessageScope::Process);
	}
}

void USGCheatManager::SetHealth(int newHealth)
{
	for (TActorIterator<ASGSpritePawn> It(GetWorld()); It; ++It)
	{
		((ASGSpritePawn*)(*It))->SetCurrentHealth(newHealth);
	}
}

void USGCheatManager::ResetGrid()
{
	for (TActorIterator<ASGGrid> It(GetWorld()); It; ++It)
	{
		((ASGGrid*)(*It))->ResetGrid();
	}
}

void USGCheatManager::TestAsyncQueue()
{
	ASGPlayerController* MyPC = GetOuterASGPlayerController();

	/* The example itself :
	*
	*  // Don't store the Queue on the stack or it will get destroyed before it finishes
	*  // You can't use "new", only a factory method "FAsyncQueue::Create()" which always returns `TSharedRef<FAsyncQueue, ESPMode::ThreadSafe>`
	*/

	for (int i = 0; i < 10; i++)
	{
		Queue->Add(FAsyncDelegate::CreateLambda([this, MyPC](const FCallbackDelegate& Callback)
		{
			UE_LOG(LogSGameAsyncTask, Warning, TEXT("Starting Long Task ASYNC"));
			FTimerHandle FooBar;
			MyPC->GetWorldTimerManager().SetTimer(FooBar, Callback, 1, false);

			// Play animation
			this->TestReplayAnimation();
		}));
	}

	/*
	Queue->Add(FAsyncQueue::MakeSync(FCallbackDelegate::CreateLambda([]()
	{
	UE_LOG(LogSGameAsyncTask, Warning, TEXT("Doing Instant Task SYNC"));
	})));

	TArray<FAsyncDelegate> ParallelTasks;
	TArray<FAsyncDelegate> LongestParallel;
	LongestParallel.Add(FAsyncDelegate::CreateLambda([this](const FCallbackDelegate& Callback)
	{
	UE_LOG(LogSGameAsyncTask, Warning, TEXT("Starting Longest Parallel ASYNC"));
	FTimerHandle FooBar;
	this->GetWorldTimerManager().SetTimer(FooBar, Callback, 10, false);
	}));

	LongestParallel.Add(FAsyncQueue::MakeSync(FCallbackDelegate::CreateLambda([]()
	{
	UE_LOG(LogSGameAsyncTask, Warning, TEXT("Finished Longest Parallel"));
	})));
	ParallelTasks.Add(FAsyncQueue::MakeSequence(LongestParallel));

	TArray<FAsyncDelegate> ShortestParallel;
	ShortestParallel.Add(FAsyncDelegate::CreateLambda([this](const FCallbackDelegate& Callback)
	{
	UE_LOG(LogSGameAsyncTask, Warning, TEXT("Starting Shortest Parallel ASYNC"));
	FTimerHandle FooBar;
	this->GetWorldTimerManager().SetTimer(FooBar, Callback, 1, false);
	}));

	ShortestParallel.Add(FAsyncQueue::MakeSync(FCallbackDelegate::CreateLambda([]()
	{
	UE_LOG(LogSGameAsyncTask, Warning, TEXT("Finished Shortest Parallel"));
	})));

	ParallelTasks.Add(FAsyncQueue::MakeSequence(ShortestParallel));
	TArray<FAsyncDelegate> MediumParallel;

	MediumParallel.Add(FAsyncDelegate::CreateLambda([this](const FCallbackDelegate& Callback)
	{
	UE_LOG(LogSGameAsyncTask, Warning, TEXT("Starting Medium Parallel ASYNC"));
	FTimerHandle FooBar;
	this->GetWorldTimerManager().SetTimer(FooBar, Callback, 2, false);
	}));

	MediumParallel.Add(FAsyncQueue::MakeSync(FCallbackDelegate::CreateLambda([]()
	{
	UE_LOG(LogSGameAsyncTask, Warning, TEXT("Finished Medium Parallel"));
	})));

	ParallelTasks.Add(FAsyncQueue::MakeSequence(MediumParallel));
	Queue->Add(FAsyncQueue::MakeParallel(ParallelTasks));

	*/

	UE_LOG(LogSGameAsyncTask, Warning, TEXT("START"));
	Queue->Execute(FCallbackDelegate::CreateLambda([this]()
	{
		this->FinishReplayAnimation();
	}));
}