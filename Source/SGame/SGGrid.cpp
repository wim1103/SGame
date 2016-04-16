// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "Math/UnrealMathUtility.h"

#include "SGGrid.h"

// Sets default values
ASGGrid::ASGGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TileManager = CreateDefaultSubobject<USGTileManager>(TEXT("TileManager"));

	TileSize.Set(106.67f, 106.67f);
}

// Called when the game starts or when spawned
void ASGGrid::BeginPlay()
{
	Super::BeginPlay();
	
	MessageEndpoint = FMessageEndpoint::Builder("Gameplay_Grid");
	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the grid needed messages
	}
}

// Called every frame
void ASGGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ASGGrid::InitGrid()
{
	GameTiles.Empty(GridWidth * GridHeight);
	GameTiles.AddUninitialized(GameTiles.Max());

	checkSlow(TileManager);
	FVector SpawnLocation;
	for (int32 column = 0; column < GridWidth; ++column)
	{
		for (int32 row = 0; row < GridHeight; ++row)
		{
			int32 TileID = TileManager->SelectTileFromLibrary();
			int32 GridAddress;
			GetGridAddressWithOffset(0, column, row, GridAddress);
			SpawnLocation = GetLocationFromGridAddress(GridAddress);

			// create the tile at the specified location
			GameTiles[GridAddress] = TileManager->CreateTile(this, SpawnLocation, GridAddress, TileID);
			if (GameTiles[GridAddress] == nullptr)
			{
				UE_LOG(LogSGame, Error, TEXT("Cannot create tile at location %d, %d"), column, row);
			}
		}
	}
}

const ASGTileBase* ASGGrid::GetTileFromGridAddress(int32 GridAddress)
{
	if (GridAddress < GameTiles.Num())
	{
		return GameTiles[GridAddress];
	}
	else
	{
		UE_LOG(LogSGame, Log, TEXT("Invalid grid address, will return null tile"));
		return nullptr;
	}
}

FVector ASGGrid::GetLocationFromGridAddress(int32 GridAddress)
{
	checkSlow(TileSize.X > 0.0f);
	checkSlow(TileSize.Y > 0.0f);
	checkSlow(GridWidth > 0);
	checkSlow(GridHeight > 0);
	FVector Center = GetActorLocation();
	FVector OutLocation = FVector(-(GridWidth / 2.0f) * TileSize.X + (TileSize.X * 0.5f), 0.0f, -(GridHeight / 2.0f) * TileSize.Y + (TileSize.Y * 0.5f));
	OutLocation.X += TileSize.X * (float)(GridAddress % GridWidth);
	OutLocation.Z += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;

	return OutLocation;
}

bool ASGGrid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32 &ReturnGridAddress)
{
	checkSlow(TileSize.X > 0.0f);
	checkSlow(TileSize.Y > 0.0f);
	checkSlow(GridWidth > 0);
	checkSlow(GridHeight > 0);
	int32 NewAxisValue;

	// Initialize to an invalid address.
	ReturnGridAddress = -1;

	// Check for going off the map in the X direction.
	NewAxisValue = (InitialGridAddress % GridWidth) + XOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridWidth - 1)))
	{
		return false;
	}

	// Check for going off the map in the Y direction.
	NewAxisValue = (InitialGridAddress / GridWidth) + YOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridHeight - 1)))
	{
		return false;
	}

	ReturnGridAddress = (InitialGridAddress + XOffset + (YOffset * GridWidth));
	checkSlow(ReturnGridAddress >= 0);
	checkSlow(ReturnGridAddress < (GridWidth * GridHeight));
	return true;
}

bool ASGGrid::AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB)
{
	if (GridAddressA == GridAddressB)
	{
		UE_LOG(LogSGame, Warning, TEXT("Pass in the same addresses"));
		return false;
	}
	else if ((FMath::Min(GridAddressA, GridAddressB) < 0) || (FMath::Max(GridAddressA, GridAddressB) >= (GridWidth * GridHeight)))
	{
		UE_LOG(LogSGame, Warning, TEXT("Pass in the invalid addresses"));
		return false;
	}

	auto AddressARow = GridAddressA / GridWidth;
	auto AddressAColumn = GridAddressA % GridWidth;
	auto AddressBRow = GridAddressB / GridWidth;
	auto AddressBColumn = GridAddressB % GridWidth;

	// The two address are neighbors only if there row and column distance less than 1
	if (FMath::Abs(AddressARow - AddressBRow) <= 1 && FMath::Abs(AddressAColumn - AddressBColumn) <= 1)
	{
		return true;
	}

	return false;
}
