// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SGame.h"
#include "SGTileBase.h"

#include "SGTileManager.generated.h"

USTRUCT(BlueprintType)
struct FSGTileType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASGTileBase> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSGTileAbilities Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSGTileData TileData;

	FSGTileType()
	{
		Probability = 1;
		TileClass = nullptr;
	}
};

/**
* Tile Manager for create and destroy tiles
*/
UCLASS(config = Game, Blueprintable)
class SGAME_API USGTileManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	ASGTileBase* CreateTile(AActor* inOwner, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);
	int32 SelectTileFromLibrary();
	bool DestroyTileWithID(int32 TileIDToDelete);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	TArray<FSGTileType> TileLibrary;

	void Initialize();
protected:
	/** Contains all the tiles in the game, including the disappering tiles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASGTileBase*> AllTiles;

private:
	int32		NextTileID;
	UWorld*		CachedWorld;
};
