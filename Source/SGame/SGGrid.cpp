// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "Math/UnrealMathUtility.h"

#include "SGGrid.h"
#include "SGGameMode.h"

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
	
	MessageEndpoint = FMessageEndpoint::Builder("Gameplay_Grid")
		.Handling<FMessage_Gameplay_LinkedTilesCollect>(this, &ASGGrid::HandleTileArrayCollect)
		.Handling<FMessage_Gameplay_NewTilePicked>(this, &ASGGrid::HandleNewTileIsPicked)
		.Handling<FMessage_Gameplay_CollectLinkLine>(this, &ASGGrid::HandleCollectLinkLine);
	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the grid needed messages
		MessageEndpoint->Subscribe<FMessage_Gameplay_LinkedTilesCollect>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_NewTilePicked>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_CollectLinkLine>();
	}

	// Initialize the grid
	GridTiles.Empty(GridWidth * GridHeight);
	GridTiles.AddZeroed(GridWidth * GridHeight);

	checkSlow(TileManager);
	TileManager->Initialize();

	// Find the link line actor in the world
	CurrentLinkLine = nullptr;
	for (TActorIterator<ASGLinkLine> It(GetWorld()); It; ++It)
	{
		if (CurrentLinkLine == nullptr)
		{
			CurrentLinkLine = *It;
		}
		else
		{
			UE_LOG(LogSGame, Warning, TEXT("There is more than more link line object in the level!"));
		}
	}
	if (CurrentLinkLine == nullptr)
	{
		UE_LOG(LogSGame, Warning, TEXT("There is no link line object in the level!"));
	}
}

// Called every frame
void ASGGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ASGGrid::Condense()
{
	TMap<int32, int32> GridHoleNumMap;

	// Iterate the each colum of grid tiles arry, find the holes
	for (int columnIndex = 0; columnIndex < GridWidth; columnIndex++)
	{
		for (int rowIndex = 0; rowIndex < GridHeight; rowIndex++)
		{
			// If it is hole already, pass it
			int gridAddress = ColumnRowToGridAddress(columnIndex, rowIndex);
			if (GridTiles[gridAddress] == nullptr)
			{
				continue;
			}

			int currentGridHoleNum = 0;
			// Search below, to find if there is any NULL tiles
			for (int j = rowIndex + 1; j < GridHeight; j++)
			{
				int testAddress = ColumnRowToGridAddress(columnIndex, j);
				if (GridTiles[testAddress] == nullptr)
				{
					currentGridHoleNum++;
				}
			}

			if (currentGridHoleNum > 0)
			{
				// Insert it into the map
				GridHoleNumMap.Add(ColumnRowToGridAddress(columnIndex, rowIndex), currentGridHoleNum);

				UE_LOG(LogSGame, Log, TEXT("Tile Address: %d will move down %d"), ColumnRowToGridAddress(columnIndex, rowIndex), currentGridHoleNum);
			}
		}
	}

	// Iterate the each column of grid tiles arry, move down the tiles
	for (int columnIndex = 0; columnIndex < GridWidth; columnIndex++)
	{
		// From down to up
		for (int rowIndex = GridHeight - 1; rowIndex >= 0; rowIndex--)
		{
			int testAddress = ColumnRowToGridAddress(columnIndex, rowIndex);
			if (GridHoleNumMap.Find(testAddress) == nullptr)
			{
				// No holes below, continue,
				continue;
			}

			int MoveDownNum = GridHoleNumMap[testAddress];

			// Send tile move message to the tile
			const ASGTileBase* testTile = GridTiles[testAddress];
			checkSlow(testTile != nullptr);

			// Update the tile new address
			FMessage_Gameplay_TileMoved* TileMoveMessage = new FMessage_Gameplay_TileMoved();
			TileMoveMessage->TileID = testTile->GetTileID();
			TileMoveMessage->OldTileAddress = testTile->GetGridAddress();
			TileMoveMessage->NewTileAddress = ColumnRowToGridAddress(columnIndex, rowIndex + MoveDownNum);

			// Publish the message
			if (MessageEndpoint.IsValid() == true)
			{
				MessageEndpoint->Publish(TileMoveMessage, EMessageScope::Process);
			}
			
			// Upate the grid address
			GridTiles[TileMoveMessage->NewTileAddress] = testTile;
			GridTiles[TileMoveMessage->OldTileAddress] = nullptr;
		}
	}

	// Refill the top empty holes
	RefillGrid();
}

void ASGGrid::RefillGrid()
{
	for (int32 Col = 0; Col < GridWidth; ++Col)
	{
		int TopGridAddress = ColumnRowToGridAddress(Col, 0);
		const ASGTileBase* CurrentColumnTopTile = GridTiles[TopGridAddress];
		if (CurrentColumnTopTile != nullptr)
		{
			// There is tile on top row, so no need to refill
			continue;
		}

		// Find how many empty space we have
		int32 RowNum = 0;
		while (RowNum < GridHeight)
		{
			int32 NewGridAddress = ColumnRowToGridAddress(Col, RowNum);
			const ASGTileBase* CurrentTile = GetTileFromGridAddress(NewGridAddress);
			if (CurrentTile == nullptr)
			{
				++RowNum;
			}
			else
			{
				break;
			}
		}

		RefillColumn(Col, RowNum);
	}

	// After all reset the tile state
	ResetTileLinkInfo();
	ResetTileSelectInfo();
}

void ASGGrid::RefillColumn(int32 inColumnIndex, int32 inNum)
{
	// We always start fill the first row [0 index row]
	for (int startRow = 0; startRow < inNum; startRow++)
	{
		// Calculate the new grid address
		int32 TileID = TileManager->SelectTileFromLibrary();
		int32 GridAddress;
		FVector SpawnLocation;
		GridAddress = ColumnRowToGridAddress(inColumnIndex, startRow);
		SpawnLocation = GetLocationFromGridAddress(GridAddress);

		// Spawn location should be moved upper
		SpawnLocation.Z += TileSize.Y * inNum;

		// create the tile at the specified location
		const ASGTileBase* NewTile = TileManager->CreateTile(this, SpawnLocation, GridAddress, TileID);
		if (NewTile == nullptr)
		{
			UE_LOG(LogSGame, Error, TEXT("Cannot create tile at location %d, %d"), inColumnIndex, startRow);
		}

		// Refill the grid with tile
		RefillGridAddressWithTile(GridAddress, NewTile);
	}
}

void ASGGrid::RefillGridAddressWithTile(int32 inGridAddress, const ASGTileBase* inTile)
{
	checkSlow(GridTiles.IsValidIndex(inGridAddress));
	checkSlow(inTile != nullptr);

	// Send the tile move message to the tile
	FMessage_Gameplay_TileMoved* TileMoveMessage = new FMessage_Gameplay_TileMoved();
	TileMoveMessage->TileID = inTile->GetTileID();
	TileMoveMessage->OldTileAddress = -1;
	TileMoveMessage->NewTileAddress = inGridAddress;

	// Publish the message
	if (MessageEndpoint.IsValid() == true)
	{
		MessageEndpoint->Publish(TileMoveMessage, EMessageScope::Process);
	}

	GridTiles[inGridAddress] = inTile;
}

const ASGTileBase* ASGGrid::GetTileFromGridAddress(int32 GridAddress)
{
	if (GridAddress < GridTiles.Num())
	{
		return GridTiles[GridAddress];
	}
	else
	{
		UE_LOG(LogSGame, Log, TEXT("Invalid grid address, will return null tile"));
		return nullptr;
	}
}

const ASGTileBase* ASGGrid::GetTileFromTileID(int32 inTileID)
{
	for (int i = 0; i < GridTiles.Num(); i++)
	{
		if (GridTiles[i] != nullptr && GridTiles[i]->GetTileID() == inTileID)
		{
			return GridTiles[i];
		}
	}

	return nullptr;
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
		// Same grid address should be the neighbors
		return true;
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

void ASGGrid::HandleTileArrayCollect(const FMessage_Gameplay_LinkedTilesCollect& Message, const IMessageContextRef& Context)
{
	for (int i = 0; i < Message.TilesAddressToCollect.Num(); i++)
	{
		int32 disappearTileAddress = Message.TilesAddressToCollect[i];
		checkSlow(GridTiles[disappearTileAddress] != nullptr);

		// Tell the tiles, it was collected
		FMessage_Gameplay_TileCollect* CollectMessage = new FMessage_Gameplay_TileCollect{ 0 };
		CollectMessage->TileID = GridTiles[disappearTileAddress]->GetTileID();
		if (MessageEndpoint.IsValid() == true)
		{
			MessageEndpoint->Publish(CollectMessage, EMessageScope::Process);
		}

		// Set null to the grid tiles array
		GridTiles[disappearTileAddress] = nullptr;
	}

	// Conden the grid
	Condense();
}

void ASGGrid::HandleNewTileIsPicked(const FMessage_Gameplay_NewTilePicked& Message, const IMessageContextRef& Context)
{
	UE_LOG(LogSGame, Log, TEXT("Player Build Path with TileID: %d"), Message.TileID);

	checkSlow(CurrentLinkLine);

	// Player's link line input should only valid in playerinput stage
	ASGGameMode* GameMode = Cast<ASGGameMode>(UGameplayStatics::GetGameMode(this));
	checkSlow(GameMode);
	if (GameMode->GetCurrentGameStatus() != ESGGameStatus::EGS_PlayerBeginInput)
	{
		// Not in the player input state, return
		return;
	}

	const ASGTileBase* CurrentTile = GetTileFromTileID(Message.TileID);
	if (CurrentTile == nullptr)
	{
		UE_LOG(LogSGame, Warning, TEXT("Cannot get tile from the tile ID %d"), Message.TileID);
		return;
	}

	// If can link to the last tile, then we build the path
	if (CanLinkToLastTile(CurrentTile) == true)
	{
		checkSlow(CurrentLinkLine);
		CurrentLinkLine->BuildPath(CurrentTile);
	}

	// Update the tile select state 
	UpdateTileSelectState();

	// Update the tile link state
	UpdateTileLinkState();
}

void ASGGrid::HandleCollectLinkLine(const FMessage_Gameplay_CollectLinkLine& Message, const IMessageContextRef& Context)
{
	checkSlow(CurrentLinkLine != nullptr);

	if (CurrentLinkLine->LinkLineTiles.Num() == 0)
	{
		UE_LOG(LogSGame, Warning, TEXT("No tiles in the link line, nothing to collect"));
		return;
	}

	// Post a tile disappear message
	FMessage_Gameplay_LinkedTilesCollect* DisappearMessage = new FMessage_Gameplay_LinkedTilesCollect();
	for (int i = 0; i < CurrentLinkLine->LinkLineTiles.Num(); i++)
	{
		checkSlow(CurrentLinkLine->LinkLineTiles[i]);
		DisappearMessage->TilesAddressToCollect.Push(CurrentLinkLine->LinkLineTiles[i]->GetGridAddress());
	}

	if (MessageEndpoint.IsValid() == true)
	{
		MessageEndpoint->Publish(DisappearMessage, EMessageScope::Process);
	}
}

void ASGGrid::UpdateTileSelectState()
{
	checkSlow(CurrentLinkLine != nullptr);

	if (CurrentLinkLine->LinkLineTiles.Num() == 0)
	{
		// No tile in the link line, so all the tiles become selectable
		ResetTileSelectInfo();
		return;
	}

	// Iterator all the grid tiles, update the tile selectable status
	for (int32 i = 0; i < 36; i++)
	{
		const ASGTileBase* testTile = GetTileFromGridAddress(i);
		checkSlow(testTile);

		FMessage_Gameplay_TileSelectableStatusChange* SelectableMessage = new FMessage_Gameplay_TileSelectableStatusChange{ 0 };
		SelectableMessage->TileID = testTile->GetTileID();
		if (CanLinkToLastTile(testTile) == true)
		{
			// The neighbor tile become selectable
			SelectableMessage->NewSelectableStatus = true;
			MessageEndpoint->Publish(SelectableMessage, EMessageScope::Process);
		}
		else
		{
			// The other tile become unselectable
			SelectableMessage->NewSelectableStatus = false;
			MessageEndpoint->Publish(SelectableMessage, EMessageScope::Process);
		}
	}
}

void ASGGrid::ResetTileSelectInfo()
{
	// Tell all the tiles that they can be selected
	if (MessageEndpoint.IsValid() == true)
	{
		FMessage_Gameplay_TileSelectableStatusChange* SelectableMessage = new FMessage_Gameplay_TileSelectableStatusChange{ 0 };

		// Set the target address to all
		SelectableMessage->TileID = -1;
		SelectableMessage->NewSelectableStatus = true;
		MessageEndpoint->Publish(SelectableMessage, EMessageScope::Process);
	}
}

void ASGGrid::UpdateTileLinkState()
{
	checkSlow(CurrentLinkLine != nullptr);

	// Iterator all the grid tiles, only the negihbor tile between the head can be selected
	checkSlow(CurrentGrid != nullptr);
	for (int32 i = 0; i < 36; i++)
	{
		const ASGTileBase* testTile = GetTileFromGridAddress(i);
		checkSlow(testTile);
		FMessage_Gameplay_TileLinkedStatusChange* SelectableMessage = new FMessage_Gameplay_TileLinkedStatusChange{ 0 };
		SelectableMessage->TileID = testTile->GetTileID();
		if (CurrentLinkLine->ContainsTileAddress(i) == true)
		{
			// Current line is linked
			SelectableMessage->NewLinkStatus = true;
			MessageEndpoint->Publish(SelectableMessage, EMessageScope::Process);
		}
		else
		{
			// Current line is not linked
			SelectableMessage->NewLinkStatus = false;
			MessageEndpoint->Publish(SelectableMessage, EMessageScope::Process);
		}
	}
}

void ASGGrid::ResetTileLinkInfo()
{
	// Tell all the tiles that they can be selected
	if (MessageEndpoint.IsValid() == true)
	{
		FMessage_Gameplay_TileLinkedStatusChange* LinkStatusChangeMessage = new FMessage_Gameplay_TileLinkedStatusChange{ 0 };

		// Set the target address to all
		LinkStatusChangeMessage->TileID = -1;
		LinkStatusChangeMessage->NewLinkStatus = false;
		MessageEndpoint->Publish(LinkStatusChangeMessage, EMessageScope::Process);
	}
}

bool ASGGrid::CanLinkToLastTile(const ASGTileBase* inCurrentTile)
{
	checkSlow(inCurrentTile);
	checkSlow(CurrentLinkLine != nullptr);
	if (CurrentLinkLine->LinkLineTiles.Num() == 0)
	{
		// First tile, can always be linked
		return true;
	}

	// Check the current tile can be linked with the last tile
	const ASGTileBase* LastTile = CurrentLinkLine->LinkLineTiles.Last();
	checkSlow(LastTile != nullptr);

	// Currently only the neighbor tiles can be selected
	if (AreAddressesNeighbors(inCurrentTile->GetGridAddress(), LastTile->GetGridAddress()) == false)
	{
		return false;
	}

	// Same tile type can always link together
	if (LastTile->Data.TileType == inCurrentTile->Data.TileType)
	{
		return true;
	}

	// Enemy links 
	if ((LastTile->Abilities.bCanLinkEnemy == true && inCurrentTile->Abilities.bEnemyTile == true) ||
		(LastTile->Abilities.bEnemyTile == true && inCurrentTile->Abilities.bCanLinkEnemy == true))
	{
		return true;
	}

	return false;
}