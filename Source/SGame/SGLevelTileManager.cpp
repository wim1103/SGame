// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGLevelTileManager.h"


// Sets default values
ASGLevelTileManager::ASGLevelTileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASGLevelTileManager::BeginPlay()
{
	Super::BeginPlay();

	// Make sure the transient tiles is deleted!
	AllTiles.Empty();
}

// Called every frame
void ASGLevelTileManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

ASGTileBase* ASGLevelTileManager::CreateTile(AActor* inOwner, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID, int32 CurrentRound)
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

		// Override the base tile data and abilities
		if (TileLibrary[TileTypeID].OverrideBaseAbilities == true)
		{
			NewTile->Abilities = TileLibrary[TileTypeID].Abilities;
		}
		if (TileLibrary[TileTypeID].OverrideBaseData == true)
		{
			NewTile->Data = TileLibrary[TileTypeID].Data;
		}

		// Of course we want to move the tile
		NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);

		NewTile->TileTypeID = TileTypeID;
		NewTile->SetGridAddress(SpawnGridAddress);
		NewTile->SetTileID(NextTileID);
		NewTile->SetSpawnedRound(CurrentRound);

		check(NextTileID < MAXINT32 - 1);
		NextTileID++;

		// Add the gamemode to the global tile array
		AllTiles.AddUnique(NewTile);

		// Cache the world pointter for delete the tile
		CachedWorld = World;

		return NewTile;
	}

	return nullptr;
}

int32 ASGLevelTileManager::SelectTileFromLibrary()
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

bool ASGLevelTileManager::DestroyTileWithID(int32 TileIDToDelete)
{
	checkSlow(CachedWorld);

	ASGTileBase* TileToDelete = nullptr;
	for (int i = 0; i < AllTiles.Num(); i++)
	{
		checkSlow(AllTiles[i] != nullptr);
		if (AllTiles[i]->GetTileID() == TileIDToDelete)
		{
			TileToDelete = AllTiles[i];
			break;
		}
	}
	if (TileToDelete == nullptr)
	{
		UE_LOG(LogSGame, Warning, TEXT("Cannot find tile id %d in the global tile array"), TileIDToDelete);
		return false;
	}

	// Destroy the tile actor
	CachedWorld->DestroyActor(TileToDelete);

	// Move it out of global tile array
	AllTiles.Remove(TileToDelete);

	return true;
}
