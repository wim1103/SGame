// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGTileManager.h"

USGTileManager::USGTileManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

ASGTileBase* USGTileManager::CreateTile(AActor* inOwner, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID)
{
	checkSlow(inOwner);
	checkSlow(TileLibrary.IsValidIndex(TileTypeID));
	checkSlow(TileLibrary[TileTypeID]);
	checkSlow(TileLibrary[TileTypeID].TileClass);

	// Check for a valid World:
	UWorld* const World = inOwner->GetWorld();
	if (World)
	{
		// Set the spawn parameters.
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = inOwner;
		SpawnParams.Instigator = nullptr;

		// Tiles never rotate
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		// Spawn the tile.
		ASGTileBase* const NewTile = World->SpawnActor<ASGTileBase>(TileLibrary[TileTypeID].TileClass, SpawnLocation, SpawnRotation, SpawnParams);
		// Of course we want to move the tile
		NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);

		NewTile->TileTypeID = TileTypeID;
		NewTile->Abilities = TileLibrary[TileTypeID].Abilities;
		NewTile->TileData = TileLibrary[TileTypeID].TileData;
		NewTile->SetGridAddress(SpawnGridAddress);
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
