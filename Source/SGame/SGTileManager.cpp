// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "TileManager.h"

USGTileManager::USGTileManager()
{
}

USGTileManager::~USGTileManager()
{
}

ASGTileBase* USGTileManager::CreateTile(TSubclassOf<class ASGTileBase> TileToSpawn, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID)
{
	if (TileToSpawn == nullptr)
	{
		return nullptr;
	}

	checkSlow(TileLibrary.IsValidIndex(TileTypeID));
	checkSlow(TileLibrary[TileTypeID]);

	// Check for a valid World:
	UWorld* const World = GetWorld();
	if (World)
	{
		// Set the spawn parameters.
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		// Tiles never rotate
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		// Spawn the tile.
		ASGTileBase* const NewTile = World->SpawnActor<ASGTileBase>(TileToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		// Of course we want to move the tile
		NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);

		NewTile->TileTypeID = TileTypeID;
		NewTile->Abilities = TileLibrary[TileTypeID].Abilities;
		NewTile->TileData = TileLibrary[TileTypeID].TileData;
		NewTile->SetGridAddress(SpawnGridAddress);
		GameTiles[SpawnGridAddress] = NewTile;
		return NewTile;
	}

	return nullptr;
}

int32 USGTileManager::SelectTileFromLibrary()
{
	float NormalizingFactor = 0;
	for (auto& TileBase : TileLibrary)
	{
		NormalizingFactor += TileBase.Probability;
	}
	float TestNumber = FMath::FRandRange(0.0f, NormalizingFactor);
	float CompareTo = 0;
	for (int32 ArrayChecked = 0; ArrayChecked != TileLibrary.Num(); ArrayChecked++)
	{
		CompareTo += TileLibrary[ArrayChecked].Probability;
		if (TestNumber <= CompareTo)
		{
			return ArrayChecked;
		}
	}
	return 0;
}

void USGTileManager::InitGridTiles()
{
	GameTiles.Empty(GridWidth * GridHeight);
	GameTiles.AddUninitialized(GameTiles.Max());
	FVector SpawnLocation;
	for (int32 column = 0; column < GridWidth; ++column)
	{
		for (int32 row = 0; row < GridHeight; ++row)
		{
			int32 TileID = SelectTileFromLibrary();
			int32 GridAddress;
			GetGridAddressWithOffset(0, column, row, GridAddress);
			SpawnLocation = GetLocationFromGridAddress(GridAddress);

			// Creathe the tile at the specified location
			CreateTile(TileLibrary[TileID].TileClass, SpawnLocation, GridAddress, TileID);
		}
	}
}

