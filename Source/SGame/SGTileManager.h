// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SGame.h"
#include "SGTileBase.h"

/**
* Tile Manager for create and destroy tiles
*/
UCLASS(config = Game, Blueprintable)
class SGAME_API USGTileManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	USGTileManager();
	~USGTileManager();

	ASGTileBase* CreateTile(TSubclassOf<class ASGTileBase> TileToSpawn, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);
	int32 SelectTileFromLibrary();
	
	/** Initialize the grid tiles */
	UFUNCTION(BlueprintCallable, Category = Tile)
	void InitGridTiles();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSGTileType> TileLibrary;
};
