#pragma once

#include "SGame.h"
#include "SGTileStructs.h"

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

/**
 * Player begin input event
*/
USTRUCT()
struct FMessage_Gameplay_PlayerBeginInput
{
	GENERATED_USTRUCT_BODY()
};

/**
 * Player end input event
*/
USTRUCT()
struct FMessage_Gameplay_PlayerEndInput
{
	GENERATED_USTRUCT_BODY()
};

/**
* Player collect link line
*/
USTRUCT()
struct FMessage_Gameplay_CollectLinkLine
{
	GENERATED_USTRUCT_BODY()
};

/**
* Player end input event
*/
USTRUCT()
struct FMessage_Gameplay_NewTilePicked
{
	GENERATED_USTRUCT_BODY()

	/** The picked tile address */
	UPROPERTY()
	int32 TileID;
};

/**
* Player collect link line
*/
USTRUCT()
struct FMessage_Gameplay_LinkedTilesCollect
{
	GENERATED_USTRUCT_BODY()

	/** The collected tile address */
	UPROPERTY()
	TArray<int32> TilesAddressToCollect;
};

/**
* Player collect link line
*/
USTRUCT()
struct FMessage_Gameplay_TileCollect
{
	GENERATED_USTRUCT_BODY()

	/** The tile id */
	UPROPERTY()
	int32 TileID;
};

/**
* Player link the tile
*/
USTRUCT()
struct FMessage_Gameplay_TileLink
{
	GENERATED_USTRUCT_BODY()

	/** The tile id */
	UPROPERTY()
	int32 TileID;
};

/**
* Damage to the tile
*/
USTRUCT()
struct FMessage_Gameplay_DamageToTile
{
	GENERATED_USTRUCT_BODY()

	/** The target tile id */
	UPROPERTY()
	int32 TileID;

	/** The damage info */
	UPROPERTY()
	TArray<FTileDamageInfo> DamageInfos;
};

/**
* Tile moved message
*/
USTRUCT()
struct FMessage_Gameplay_TileMoved
{
	GENERATED_USTRUCT_BODY()

	/** The tile id */
	UPROPERTY()
	int32 TileID;

	/** The old grid address */
	UPROPERTY()
	int32 OldTileAddress;

	/** The old grid address */
	UPROPERTY()
	int32 NewTileAddress;
};

/**
* The tile selected status change event
*/
USTRUCT()
struct FMessage_Gameplay_TileSelectableStatusChange
{
	GENERATED_USTRUCT_BODY()

	/** The picked tile address, if the address is -1, then all apply to all tiles*/
	UPROPERTY()
	int32 TileID;

	/** The new selectable status, it can be override by the tile logic itself*/
	UPROPERTY()
	bool NewSelectableStatus;
};

/**
* The tile linked status change event
*/
USTRUCT()
struct FMessage_Gameplay_TileLinkedStatusChange
{
	GENERATED_USTRUCT_BODY()

	/** The picked tile address, if the address is -1, then all apply to all tiles*/
	UPROPERTY()
	int32 TileID;

	/** The new selectable status, it can be override by the tile logic itself*/
	UPROPERTY()
	bool NewLinkStatus;
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
		EGS_PlayerTurnBegin,		// Player turn begin
		EGS_PlayerRegengerate,		// Player regenerate
		EGS_PlayerSkillCD,			// Player regenerate
		EGS_PlayerBeginInput,		// Player begin input, can link line or use the skill
		EGS_PlayerEndBuildPath,		// Player end build the path, but he can still use skill or buy some thing
		EGS_PlayerEndInput,			// Player end input, his turn end
		EGS_EnemyAttack,			// Enemy attack player
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

/**
* Enemy begin attack
*/
USTRUCT()
struct FMessage_Gameplay_EnemyBeginAttack
{
	GENERATED_USTRUCT_BODY()
};

/**
* Enemy begin attack
*/
USTRUCT()
struct FMessage_Gameplay_EnemyGetHit
{
	GENERATED_USTRUCT_BODY()

	/** The picked tile address, if the address is -1, then all apply to all tiles*/
	UPROPERTY()
	int32 TileID;
};