// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGTileBase.h"
#include "SGGrid.h"

// Sets default values
ASGTileBase::ASGTileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// We want the tile can be moved (falling), so we need a root component
	SetRootComponent(GetRenderComponent());
}

// Called when the game starts or when spawned
void ASGTileBase::BeginPlay()
{
	Super::BeginPlay();

	// Set our class up to handle clicks and touches.
	OnClicked.AddUniqueDynamic(this, &ASGTileBase::TilePress_Mouse);
	OnBeginCursorOver.AddUniqueDynamic(this, &ASGTileBase::TileEnter_Mouse);

	OnInputTouchBegin.AddUniqueDynamic(this, &ASGTileBase::TilePress);
	OnInputTouchEnter.AddUniqueDynamic(this, &ASGTileBase::TileEnter);

	FString EndPointName = FString::Printf(TEXT("Gameplay_Tile_%d"), GridAddress);
	MessageEndpoint = FMessageEndpoint::Builder(*EndPointName)
		.Handling<FMessage_Gameplay_TileSelectableStatusChange>(this, &ASGTileBase::HandleSelectableStatusChange)
		.Handling<FMessage_Gameplay_TileLinkedStatusChange>(this, &ASGTileBase::HandleLinkStatusChange)
		.Handling<FMessage_Gameplay_TileMoved>(this, &ASGTileBase::HandleTileMove);
	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the tile need events
		MessageEndpoint->Subscribe<FMessage_Gameplay_TileSelectableStatusChange>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_TileLinkedStatusChange>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_TileMoved>();
	}

	Grid = Cast<ASGGrid>(GetOwner());
}

// Called every frame
void ASGTileBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	TickFalling(DeltaTime);
}

void ASGTileBase::TilePress(ETouchIndex::Type FingerIndex)
{
	UE_LOG(LogSGame, Log, TEXT("Tile %s was pressed, address (%d,%d)"), *GetName(), GridAddress % 6, GridAddress / 6);

	// Tell the game logic, the new tile is picked
	FMessage_Gameplay_NewTilePicked* TilePickedMessage = new FMessage_Gameplay_NewTilePicked();
	TilePickedMessage->TileAddress = GridAddress;
	if (MessageEndpoint.IsValid() == true)
	{
		MessageEndpoint->Publish(TilePickedMessage, EMessageScope::Process);
	}
}

void ASGTileBase::TileEnter(ETouchIndex::Type FingerIndex)
{
	UE_LOG(LogSGame, Log, TEXT("Tile %s was entered, address (%d,%d)"), *GetName(), GridAddress % 6, GridAddress / 6);
}

void ASGTileBase::TilePress_Mouse()
{
	TilePress(ETouchIndex::Touch1);
}

void ASGTileBase::TileEnter_Mouse()
{
	// This is meant to simulate finger-swiping, so ignore if the mouse isn't clicked.
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			TileEnter(ETouchIndex::Touch1);
		}
	}
}

bool ASGTileBase::IsSelectable() const
{
	return TileData.TileStatusArray.Contains(ESGTileStatusFlag::ESF_SELECTABLE);
}

void ASGTileBase::SetGridAddress(int32 NewLocation)
{
	GridAddress = NewLocation;
}

int32 ASGTileBase::GetGridAddress() const
{
	return GridAddress;
}

void ASGTileBase::StartFalling()
{
	/*
	if (!bUseCurrentWorldLocation)
	{
		// Fall from where we are on the grid to where we are supposed to be on the grid.
		int32 YOffset = 0;
		int32 HeightAboveBottom = 1;
		while (true)
		{
			++YOffset;

			// Try to move down 1 tile distance
			if (Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress))
			{
				if (ASGTileBase* TileBelow = Grid->GetTileFromGridAddress(LandingGridAddress))
				{
					// We're not off the grid, so check to see what is in this space and react to it.
					if (TileBelow->TileState == ETileState::ETS_Falling)
					{
						// This space contains a falling tile, so continue to fall through it, but note that the tile will land underneath us, so we need to leave a gap for it.
						++HeightAboveBottom;
						continue;
					}
					else if (TileBelow->TileState == ETileState::ETS_PendingDelete)
					{
						// This space contains a tile that is about to be deleted. We can fall through this space freely.
						continue;
					}
				}
				else
				{
					// The space below is empty, but is on the grid. We can fall through this space freely.
					continue;
				}
			}

			// This space is off the grid or contains a tile that is staying. Go back one space and stop.
			YOffset -= HeightAboveBottom;
			Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress);
			break;
		}
		FallDistance = Grid->TileSize.Y * YOffset;
		FallingEndLocation = FallingStartLocation;
		FallingEndLocation.Z -= FallDistance;
	}
	else
	{
		// Fall from where we are physically to where we are supposed to be on the grid.
		LandingGridAddress = GetGridAddress();
		FallingEndLocation = Grid->GetLocationFromGridAddress(LandingGridAddress);
		FallDistance = FallingStartLocation.Z - FallingEndLocation.Z;
	}

	AMatch3GameMode* CurrentGameMode = Cast<AMatch3GameMode>(UGameplayStatics::GetGameMode(this));
	TotalFallingTime = 0.0f;
	if (CurrentGameMode && (CurrentGameMode->TileMoveSpeed > 0.0f))
	{
		TotalFallingTime = FallDistance / CurrentGameMode->TileMoveSpeed;
	}
	if (TotalFallingTime <= 0.0f)
	{
		TotalFallingTime = 0.75f;
	}
	StartFallingEffect();
	*/
}

void ASGTileBase::TickFalling(float DeltaTime)
{

}

void ASGTileBase::HandleSelectableStatusChange(const FMessage_Gameplay_TileSelectableStatusChange& Message, const IMessageContextRef& Context)
{
	if (Message.TileAddress != -1 && Message.TileAddress != GridAddress)
	{
		// Selectable event is not sent to this tile
		return;
	}

	UE_LOG(LogSGame, Log, TEXT("Tile %d selectable flag changed to %d"), GridAddress, Message.NewSelectableStatus);

	if (Message.NewSelectableStatus == true)
	{
		// Add the selectable flag to the status array
		TileData.TileStatusArray.AddUnique(ESGTileStatusFlag::ESF_SELECTABLE);

		// Set the white color 
		GetRenderComponent()->SetSpriteColor(FLinearColor::White);
	}
	else
	{
		// Remove the selectable flag
		TileData.TileStatusArray.Remove(ESGTileStatusFlag::ESF_SELECTABLE);

		// Dim the sprite
		GetRenderComponent()->SetSpriteColor(FLinearColor(0.2f, 0.2f, 0.2f));
	}
}

void ASGTileBase::HandleLinkStatusChange(const FMessage_Gameplay_TileLinkedStatusChange& Message, const IMessageContextRef& Context)
{
	if (Message.TileAddress != -1 && Message.TileAddress != GridAddress)
	{
		// Selectable event is not sent to this tile
		return;
	}

	UE_LOG(LogSGame, Log, TEXT("Tile %d link status changed to %d"), GridAddress, Message.NewLinkStatus);

	if (Message.NewLinkStatus == true)
	{
		// Add the selectable flag to the status array
		TileData.TileStatusArray.AddUnique(ESGTileStatusFlag::ESF_LINKED);

		// Set the linked sprite
		GetRenderComponent()->SetSprite(Sprite_Selected);
	}
	else
	{
		// Remove the selectable flag
		TileData.TileStatusArray.Remove(ESGTileStatusFlag::ESF_LINKED);

		// Set the normal sprite
		GetRenderComponent()->SetSprite(Sprite_Normal);
	}
}

void ASGTileBase::HandleTileMove(const FMessage_Gameplay_TileMoved& Message, const IMessageContextRef& Context)
{
	float FallDistance = 0;
	FallingStartTime = GetWorld()->GetTimeSeconds();
	FallingStartLocation = GetActorLocation();

	check(Grid);
	FallingEndLocation = Grid->GetLocationFromGridAddress(Message.NewTileAddress);
	FallDistance = FallingStartLocation.Z - FallingEndLocation.Z;
	TotalFallingTime = FallDistance / FallingSpeed;
}

