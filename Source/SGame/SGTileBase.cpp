// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGTileBase.h"

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
	
}

// Called every frame
void ASGTileBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASGTileBase::TilePress(ETouchIndex::Type FingerIndex)
{
	// We clicked or touched the tile.
// 	if (!UGameplayStatics::IsGamePaused(this) && Grid)
// 	{
// 		// Grid->OnTileWasSelected(this);
// 	}
}

void ASGTileBase::TileEnter(ETouchIndex::Type FingerIndex)
{
	// We have moved into the tile's space while we had a different tile selected. This is the same as pressing the tile directly.
	// Note that we need to make sure it's a different actual tile (i.e. not NULL) because deselecting a tile by touching it twice will then trigger the TileEnter event and re-select it.
	// if (!UGameplayStatics::IsGamePaused(this) && Grid)
	{
		// ATile* CurrentlySelectedTile = Grid->GetCurrentlySelectedTile();
// 		if (CurrentlySelectedTile && (CurrentlySelectedTile != this))
// 		{
// 			TilePress(FingerIndex);
// 		}
	}
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

void ASGTileBase::SetGridAddress(int32 NewLocation)
{
	GridAddress = NewLocation;
}

int32 ASGTileBase::GetGridAddress() const
{
	return GridAddress;
}

