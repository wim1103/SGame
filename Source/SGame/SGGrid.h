// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "SGTileBase.h"

#include "SGGrid.generated.h"

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

UCLASS()
class SGAME_API ASGGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGGrid(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASGTileBase*> GameTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSGTileType> TileLibrary;

	/** The size of a space on the grid. Does not include borders or spacing between tiles. */
	UPROPERTY(EditAnywhere, Category = Tile)
	FVector2D TileSize;

	/** The width of the grid. Needed to calculate tile positions and neighbors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32 GridWidth;

	/** The height of the grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32 GridHeight;

	ASGTileBase* CreateTile(TSubclassOf<class ASGTileBase> TileToSpawn, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);
	int32 SelectTileFromLibrary();

	/** Initialize the tiles on the grid*/
	UFUNCTION(BlueprintCallable, Category = Initialization)
	void InitGrid();

	/** Get the world location for a given grid address. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	FVector GetLocationFromGridAddress(int32 GridAddress);
	FVector GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles);

	/** Get a grid address relative to another grid address. Offset between addresses is measured in tiles. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32 &ReturnGridAddress);
};
