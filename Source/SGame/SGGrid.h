// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"

#include "SGTileBase.h"
#include "Messaging.h"
#include "SGameMessages.h"
#include "SGLevelTileManager.h"
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

	/** Reset grid */
	UFUNCTION(BlueprintCallable, Category = Refill)
	void ResetGrid();

	/** Refill all the tiles on the grid*/
	UFUNCTION(BlueprintCallable, Category = Refill)
	void RefillGrid();

	/** Reset the tile select info */
	UFUNCTION(BlueprintCallable, Category = Grid)
	void ResetTiles();

	/** Get the tile by the address*/
	ASGTileBase* GetTileFromGridAddress(int32 GridAddress);
	/** Get the tile by the tile id*/
	ASGTileBase* GetTileFromTileID(int32 inTileID);

	/** Get the world location for a given grid address. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	FVector GetLocationFromGridAddress(int32 GridAddress);

	/** Get a grid address relative to another grid address. Offset between addresses is measured in tiles. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32 &ReturnGridAddress);

	/** Get a tile from column and row*/
	UFUNCTION(BlueprintCallable, Category = Tile)
	ASGTileBase* GetTileFromColumnAndRow(int32 inColumn, int32 inRow);

	/** Get a tile square from column and row and its surronding tiles*/
	UFUNCTION(BlueprintCallable, Category = Tile)
	TArray<ASGTileBase*> GetTileSquareFromColumnAndRow(int32 inColumn, int32 inRow);

	/** Take into the column and row, return the grid address*/
	int32 ColumnRowToGridAddress(int columnIndex, int32 rowIndex)
	{
		checkSlow(columnIndex < GridWidth && rowIndex < GridHeight);
		return (GridHeight - rowIndex - 1) * GridWidth + columnIndex;
	}

	/** Helper to get tile manager */
	ASGLevelTileManager* GetTileManager() const 
	{  
		checkSlow(LevelTileManager);
		return LevelTileManager; 
	}

	/** Is some tile is moving */
	UFUNCTION(BlueprintCallable, Category = Tile)
	bool IsSomeTileFalling() { return CurrentFallingTileNum > 0; }

	/** Calculate if the two address are neighbor, the link is 8 directions*/
	UFUNCTION(BlueprintCallable, Category = Tile)
	bool AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB);

	// Start Attack, using BP function to implement, since it is more convenient to polish
	UFUNCTION(BlueprintImplementableEvent)
	void StartAttackFadeAnimation();

	const TArray<ASGTileBase*>& GetGridTiles() { return GridTiles; }

protected:
	/** Contains the tile only on the grid */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASGTileBase*> GridTiles;

	/** Reset the tile select info */
	UFUNCTION(BlueprintCallable, Category = Grid)
	void ResetTileSelectInfo();

	/** Reset the tile link info */
	UFUNCTION(BlueprintCallable, Category = Grid)
	void ResetTileLinkInfo();

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
	void RefillGridAddressWithTile(int32 inGridAddress, ASGTileBase* inTile);

	/** The size of a space on the grid. Does not include borders or spacing between tiles. */
	UPROPERTY(EditAnywhere, Category = Tile)
	FVector2D TileSize;

	/** Current falling tile num */
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 CurrentFallingTileNum;

	/** The width of the grid. Needed to calculate tile positions and neighbors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32 GridWidth;

	/** The height of the grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32 GridHeight;

	/** Level tile manager class for this grid*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManager)
	TSubclassOf<class ASGLevelTileManager> LevelTileManagerClass;
	
	/** Actual level tile manager class for this grid*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManager)
	ASGLevelTileManager* LevelTileManager;
private:
	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;

	/** Handles the player picked new tile*/
	void HandleNewTileIsPicked(const FMessage_Gameplay_NewTilePicked& Message, const IMessageContextRef& Context);

	/** Handle tile grid event*/
	void HandleTileArrayCollect(const FMessage_Gameplay_LinkedTilesCollect& Message, const IMessageContextRef& Context);

	/** Handle when some tile begin move, just increase the count*/
	void HandleTileBeginMove(const FMessage_Gameplay_TileBeginMove& Message, const IMessageContextRef& Context);

	/** Handle when some tile end move, just decrease the count*/
	void HandleTileEndMove(const FMessage_Gameplay_TileEndMove& Message, const IMessageContextRef& Context);

	void UpdateTileSelectState();
	void UpdateTileLinkState();
	
	ASGLinkLine* CurrentLinkLine;
};
