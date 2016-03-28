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

void ASGTileBase::SetGridAddress(int32 NewLocation)
{
	GridAddress = NewLocation;
}

int32 ASGTileBase::GetGridAddress() const
{
	return GridAddress;
}

