// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperSpriteActor.h"
#include "GameFramework/Actor.h"
#include "Messaging.h"
#include "SGameMessages.h"

#include "SGTileBase.generated.h"

class ASGGrid;

#define  FILTER_MESSAGE \
	if (FilterMessage(Message.TileID) == false) \
	return;


USTRUCT()
struct FSGTileData
{
	GENERATED_USTRUCT_BODY();

public:
	/** The base type of the current tile*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESGTileType::Type> TileType;

	/** The current tile status flag, should not be accessed anywhere, for test convenient now*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< TEnumAsByte<ESGTileStatusFlag::Type> > TileStatusArray;

	/** The current tile resource info*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileResourceUnit> TileResourceArray;

	/** The current tile damage info, only valid if the tile can cause damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTileDamageInfo CauseDamageInfo;

	/** The current tile damage info, only valid if the tile can take damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTileLifeArmorInfo LifeArmorInfo;
};

UCLASS()
class SGAME_API ASGTileBase : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGTileBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** When a tile is touched. */
	UFUNCTION()
	void TilePress(ETouchIndex::Type FingerIndex);

	/** When the user's finger moves over a tile. */
	UFUNCTION()
	void TileEnter(ETouchIndex::Type FingerIndex);

	/** When the user's finger release a tile*/
	UFUNCTION()
	void TileRelease(ETouchIndex::Type FingerIndex);

	/** Mouse surrogate for TilePress. */
	UFUNCTION()
	void TilePress_Mouse();

	/** Mouse surrogate for TileEnter. */
	UFUNCTION()
	void TileEnter_Mouse();

	/** Mouse surrogate for TileRelease*/
	UFUNCTION()
	void TileRelease_Mouse();

	/** Is current tile selecatable*/
	UFUNCTION()
	bool IsSelectable() const;

	void SetGridAddress(int32 NewLocation);
	int32 GetGridAddress() const;

	int32 GetTileID() const { return TileID; }
	void SetTileID(int32 val) { TileID = val; }

	virtual int32 GetTileCausedDamage() const { return Data.CauseDamageInfo.InitialDamage; }

	int32 GetSpawnedRound() const { return SpawnedRound; }
	void SetSpawnedRound(int32 val) { SpawnedRound = val; }

	UPROPERTY(BlueprintReadOnly)
	int32 TileTypeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSGTileAbilities Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSGTileData Data;

	// Falling functions
	void StartFalling();
	void FinishFalling();
	
	UFUNCTION()
	void TickFalling(float DeltaTime);

	// Currently all the tile can be collect, even the enemy tile, because it can 
	// be part of the XP resouces
	virtual void OnTileCollected();

	// Currenttly only the enemy tile can take damage
	virtual void OnTileTakeDamage();

	/** Return the tile resource that can be collect */
	virtual TArray<FTileResourceUnit> GetTileResource() const;

	/**
	* Evaluate the if the tile survive after this damage
	*
	* @param DamageInfos all the damage infos caused to the tiles
	*
	* @return return true means that the tile is dead (life reduce to 0)
	*/
	bool EvaluateDamageToTile(const TArray<FTileDamageInfo>& DamageInfos) const;
	

protected:
	/** Location on the grid as a 1D key/value. To find neighbors, ask the grid. */
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 GridAddress;

	/** How quickly tiles slide into place. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	float FallingSpeed;

	/** Location where we will land on the grid as a 1D key/value. Used while falling. */
	int32 LandingGridAddress;

	// The sprite asset for link corners 45 degree
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Sprite_Selected;

	// The sprite asset for link corners 45 degree
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Sprite_Normal;

	// Handle falling
	float TotalFallingTime;
	float FallingElapsedTime;
	FVector FallingStartLocation;
	FVector FallingEndLocation;

	/** Current tile's Id */
	int32 TileID;

	/** Spawned turn */
	int32 SpawnedRound;

	/** Keep a weak reference to the owner*/
	ASGGrid* Grid;

	/**
	* Called when the tile take damage
	*
	* @param DamageInfos	all the damage infos caused to the tiles
	* @param LifeArmorInfo	the life armor info to take damage
	*
	* @return return true means that the tile is dead (life reduce to 0)
	*/
	virtual bool OnTakeTileDamage(const TArray<FTileDamageInfo>& DamageInfos, FTileLifeArmorInfo& LifeArmorInfo) const;

	/** If the Message send to me */
	bool FilterMessage(int32 inTileID)
	{
		if (inTileID == -1)
		{
			// It send to all tiles
			return true;
		}

		return inTileID == TileID;	
	}

	FMessage_Gameplay_DamageToTile CachedDamageMessage;

private:

	// Holds the messaging endpoint.
	// Note that we don't want the sub class inherited this member,
	// because every message handler should explicitly handled by itself class
	FMessageEndpointPtr MessageEndpoint;

	/** Handles tile become selectalbe */
	void HandleSelectableStatusChange(const FMessage_Gameplay_TileSelectableStatusChange& Message, const IMessageContextRef& Context);

	/** Handles tile become selectalbe */
	void HandleLinkStatusChange(const FMessage_Gameplay_TileLinkedStatusChange& Message, const IMessageContextRef& Context);

	/** Handles tile become selectalbe */
	void HandleTileMove(const FMessage_Gameplay_TileMoved& Message, const IMessageContextRef& Context);

	/** Handle tile collected */
	void HandleTileCollected(const FMessage_Gameplay_TileCollect& Message, const IMessageContextRef& Context);

	/** Handle tile linked */
	void HandleTileLinked(const FMessage_Gameplay_TileCollect& Message, const IMessageContextRef& Context);

	/** Handle take damage message */
	void HandleTakeDamage(const FMessage_Gameplay_DamageToTile& Message, const IMessageContextRef& Context);
};
