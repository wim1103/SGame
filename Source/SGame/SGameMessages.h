#pragma once

#include "SGame.h"
#include "SGameMessages.generated.h"

/**
*/
USTRUCT()
struct FMessage_Gameplay_GameStart
{
	GENERATED_USTRUCT_BODY()
};


/**
*/
USTRUCT()
struct FMessage_Gameplay_GameOver
{
	GENERATED_USTRUCT_BODY()
};

/** Defines the game state */
UENUM()
namespace ESGGameStatus
{
	enum Type
	{
		EGS_Init,					// Initial value
		EGS_GameStart,				// Shield
		EGS_RondBegin,				// New round begin
		EGS_PlayerTurnBegin,		// Player Turn begin
		EGS_RoundEnd,				// End of the round
		EGS_GameOver,				// Game over
	};
}

/**
 * Game status update messages
*/
USTRUCT()
struct FMessage_Gameplay_GameStatusUpdate
{
	GENERATED_USTRUCT_BODY()

	/** The new game status */
	UPROPERTY()
	TEnumAsByte<ESGGameStatus::Type> NewGameStatus;
};
