// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SGTileBase.h"

#include "SGLevelTileManager.generated.h"

UCLASS()
class SGAME_API ASGLevelTileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGLevelTileManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	ASGTileBase* CreateTile(AActor* inOwner, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID, int32 CurrentRound);
	int32 SelectTileFromLibrary();
	bool DestroyTileWithID(int32 TileIDToDelete);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManager)
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
