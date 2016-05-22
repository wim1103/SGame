// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"

#include "SGTileBase.h"
#include "Messaging.h"
#include "SGameMessages.h"
#include "SGTileManager.h"
#include "SGLinkLine.h"

#include "SGGrid.generated.h"

UCLASS()
class SGAME_API ASGGrid : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Refill all the tiles on the grid*/
	UFUNCTION(BlueprintCallable, Category = Refill)
	void RefillGrid();

	/** Reset the tile select info */
	UFUNCTION(BlueprintCallable, Category = Grid)
	void ResetTiles();

	/** Get the tile by the address*/
	const ASGTileBase* GetTileFromGridAddress(int32 GridAddress);
	/** Get the tile by the tile id*/
	const ASGTileBase* GetTileFromTileID(int32 inTileID);

	/** Get the world location for a given grid address. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	FVector GetLocationFromGridAddress(int32 GridAddress);

	/** Get a grid address relative to another grid address. Offset between addresses is measured in tiles. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32 &ReturnGridAddress);

	/** Calculate if the two address are neighbor, the link is 8 directions*/
	bool AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB);

	/** Take into the column and row, return the grid address*/
	int32 ColumnRowToGridAddress(int columnIndex, int32 rowIndex)
	{
		checkSlow(columnIndex < GridWidth && rowIndex < GridHeight);
		return (GridHeight - rowIndex - 1) * GridWidth + columnIndex;
	}

	USGTileManager*	GetTileManager() const { return TileManager; }
protected:
	/** Contains the tile only on the grid */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<const ASGTileBase*> GridTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APaperSpriteActor*			AttackFadingSprite;

	/** Return the tile can link to the linkline */
	UFUNCTION(BlueprintCallable, Category = LinkLine)
	bool CanLinkToLastTile(const ASGTileBase* inCurrentTile);

	/** Reset the tile select info */
	UFUNCTION(BlueprintCallable, Category = Grid)
	void ResetTileSelectInfo();

	/** Reset the tile link info */
	UFUNCTION(BlueprintCallable, Category = Grid)
	void ResetTileLinkInfo();

	/** Tick attacking */
	UFUNCTION()
	void TickAttackFadeAnimation(float DeltaSeconds);

	/** Tick attacking */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BeginAttackFadeAnimation();

	/** Tick attacking */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void EndAttackFadeAnimation();

	/** How long will it takes to finish a attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackingDuration;
	float AttackingElapsedTime;
	bool IsAttacking;

	/** The time window to fade in and out */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float FadingTimeWindow;

	/** The fading alpha */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ResultFadingAlpha;

	/** Condense the grid, fill the holes*/
	UFUNCTION(BlueprintCallable, Category = Refill)
	void Condense();

	/** Refill one column on the grid
	*
	* @param inColumnIndex columnIndex to fill
	* @param inNum		   how many rows to fill
	*
	*/
	UFUNCTION(BlueprintCallable, Category = Refill)
	void RefillColumn(int32 inColumnIndex, int32 inNum);

	/** Refill a specific grid address with the tile */
	UFUNCTION(BlueprintCallable, Category = Refill)
	void RefillGridAddressWithTile(int32 inGridAddress, const ASGTileBase* inTile);

	/** The size of a space on the grid. Does not include borders or spacing between tiles. */
	UPROPERTY(EditAnywhere, Category = Tile)
	FVector2D TileSize;

	/** The width of the grid. Needed to calculate tile positions and neighbors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32 GridWidth;

	/** The height of the grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32 GridHeight;

	/** Tile Manager for this grid*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
	USGTileManager*		TileManager;

	/** 
	 * Calculate the enemy damage 
	 *
	 * @param outDamageCanBeShield damage can be shielded
	 * @param outDamageDirectToHP damage direct to the hp
	 *
	 * @return true means there is enemy who will attack the player
	*/
	UFUNCTION(BlueprintCallable, Category = Attack)
	bool CalculateEnemyDamageToPlayer(float& outDamageCanBeShield, float& outDamageDirectToHP);
private:
	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;

	/** Handles the player picked new tile*/
	void HandleNewTileIsPicked(const FMessage_Gameplay_NewTilePicked& Message, const IMessageContextRef& Context);

	/** Handle collect the link line*/
	void HandleCollectLinkLine(const FMessage_Gameplay_CollectLinkLine& Message, const IMessageContextRef& Context);

	/** Handle tile grid event*/
	void HandleTileArrayCollect(const FMessage_Gameplay_LinkedTilesCollect& Message, const IMessageContextRef& Context);

	/** Handle begin attack event*/
	void HandleBeginAttack(const FMessage_Gameplay_EnemyBeginAttack& Message, const IMessageContextRef& Context);

	void UpdateTileSelectState();
	void UpdateTileLinkState();
	
	ASGLinkLine* CurrentLinkLine;
};
