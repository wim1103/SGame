// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"
#include "Messaging.h"

#include "SGTileBase.h"
#include "SGameMessages.h"
#include "SGLinkLine.h"
#include "SGGrid.h"
#include "SGTileManager.h"
#include "SGSpritePawn.h"
#include "SGPlayerSkillManager.h"

#include "SGGameMode.generated.h"

/**
 * The Gameplay mode
 */
UCLASS()
class SGAME_API ASGGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASGGameMode(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/** Initialize the tiles on the grid*/
	UFUNCTION(BlueprintCallable, Category = Game)
	void SetNextStatus(ESGGameStatus::Type inNewStatus);

	/** Initialize the tiles on the grid*/
	UFUNCTION(BlueprintCallable, Category = Game)
	ESGGameStatus::Type GetCurrentGameStatus();

	/** Begin the new round */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnBeginRound();

	/** Player turn begin */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnPlayerTurnBegin();

	/** Player regenerate */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnPlayerRegenerate();

	/** Player skil CD */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnPlayerSkillCD();

	/** Player input */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnPlayerBeginInputStage();

	/** Player end build path */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnPlayerEndBuildPathStage();
	FTimerHandle PlayerEndInputTimer;
	void TimerPlayerEndInput();

	/** Player end input */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnPlayerEndInputStage();

	/** Enemy attack stage*/
	void OnEnemyAttackStage();

	/** Called when this round end*/
	void OnRoundEndStage();

	/** Called when game over */
	UFUNCTION(BlueprintCallable, Category = Game)
	void OnGameOver();

	/** Create player skill */
	UFUNCTION(BlueprintCallable, Category = Game)
	ASGSkillBase* CreatePlayerSkilkByName(FName inSkillName);

	/** Check if game over */
	bool CheckGameOver();

	/** Override the parent tick to do some customized tick operations*/
	virtual void Tick(float DeltaSeconds) override;

	ASGGrid* GetCurrentGrid() const { return CurrentGrid; }
	void SetCurrentGrid(ASGGrid* val) { checkSlow(val != nullptr);  CurrentGrid = val; }

	int32 GetCurrentRound() const { return CurrentRound; }

	UFUNCTION(BlueprintCallable, Category = Game)
	bool IsLinkLineValid();

	bool ShouldReplayLinkAnimation() const { return bShouldReplayLinkAnimation; }

protected:

	/** The minum lenth require for on valid link line*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	int32 MinimunLengthLinkLineRequired;

	/** Whether to replay the link animation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool bShouldReplayLinkAnimation;

	/** Whether to replay the link animation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	USGPlayerSkillManager* PlayerSkillManager;

private:
	/** Handles Game start messages. */
	void HandleGameStart(const FMessage_Gameplay_GameStart& Message, const IMessageContextRef& Context);

	/** Handles the game status update messages. */
	void HandleGameStatusUpdate(const FMessage_Gameplay_GameStatusUpdate& Message, const IMessageContextRef& Context);

	ESGGameStatus::Type CurrentGameGameStatus;
	ESGGameStatus::Type NextGameStatus;

	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;

	/** Current round number*/
	int32				CurrentRound;

	/** Current link line */
	ASGLinkLine*		CurrentLinkLine;

	/** Current grid */
	ASGGrid*			CurrentGrid;

	/** Current player pawn (master) */
	ASGSpritePawn*		CurrentPlayerPawn;
};
