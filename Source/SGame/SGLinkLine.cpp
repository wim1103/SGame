// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGGameMode.h"
#include "SGLinkLine.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASGLinkLine::ASGLinkLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Movable;

	HeadSpriteRenderComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LinkLineSpriteComponent-Head"));
	HeadSpriteRenderComponent->Mobility = EComponentMobility::Movable;
	HeadSpriteRenderComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HeadSpriteRenderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TailSpriteRenderComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LinkLineSpriteComponent-Tail"));
	TailSpriteRenderComponent->Mobility = EComponentMobility::Movable;
	TailSpriteRenderComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TailSpriteRenderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ReplayHeadAnimationDuration = 0.3f;
	ReplayHeadAnimationElapsedTime = 0;
}

// Called when the game starts or when spawned
void ASGLinkLine::BeginPlay()
{
	Super::BeginPlay();
	
	// Build the link line message endpoint
	MessageEndpoint = FMessageEndpoint::Builder("Gameplay_LinkLine");

	// Find the grid actor in the world
	ParentGrid = nullptr;
	for (TActorIterator<ASGGrid> It(GetWorld()); It; ++It)
	{
		if (ParentGrid == nullptr)
		{
			ParentGrid = *It;
		}
		else
		{
			UE_LOG(LogSGame, Warning, TEXT("There is more than more grid object in the level!"));
		}
	}
	if (ParentGrid == nullptr)
	{
		UE_LOG(LogSGame, Error, TEXT("There is no grid object in the level!"));
	}
}

// Called every frame
void ASGLinkLine::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	TickReplayLinkHeadAnimation(DeltaTime);
}

bool ASGLinkLine::UpdateLinkLineSprites()
{
	if (bIsStaticLine == true)
	{
		if (StaticLinePoints.Num() < 2)
		{
			return false;
		}

		return UpdateLinkLineSprites(StaticLinePoints);
	}

	return UpdateLinkLineSprites(LinkLinePoints);
}

bool ASGLinkLine::UpdateLinkLineSprites(const TArray<int32>& LinePoints)
{
	// Clean the body sprites
	for (int32 i = 0; i < LinkLineSpriteRendererArray.Num(); i++)
	{
		checkSlow(LinkLineSpriteRendererArray[i] != nullptr);
		// Detach the component
		LinkLineSpriteRendererArray[i]->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		LinkLineSpriteRendererArray[i]->UnregisterComponent();
	}
	LinkLineSpriteRendererArray.Empty();

	if (LinkLinePoints.Num() < 2)
	{
		// Only one point cannot become a link line

		// Hide the head and tail component
		HeadSpriteRenderComponent->SetVisibility(false);
		TailSpriteRenderComponent->SetVisibility(false);

		return true;
	}
	else
	{
		// Unhide the head and tail component
		HeadSpriteRenderComponent->SetVisibility(true);
		TailSpriteRenderComponent->SetVisibility(true);
	}

	// The link line should scale to 1.5 to fit the grid
	RootComponent->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));

	// Iterate all points, and generate line between the two points
	ELinkDirection LastDirection = ELD_Begin;
	for (int32 i = 0; i < LinePoints.Num(); i++)
	{
		// For the first point, we just mark down the initial position
		FVector InitialTileCorrds;
		if (i == 0)
		{
			InitialTileCorrds.X = LinePoints[0] % 6;
			InitialTileCorrds.Y = LinePoints[0] / 6;

			// Set the link line at the head position
			FVector LinkLineWorldLocation = RootComponent->GetComponentLocation();
			LinkLineWorldLocation.X = (InitialTileCorrds.X + 0.5f - 3) * 106.0f;
			LinkLineWorldLocation.Z = (InitialTileCorrds.Y + 0.5f - 3) * 106.0f;
			RootComponent->SetWorldLocation(LinkLineWorldLocation);

			// Continue from points 1
			continue;
		}

		auto CurrentTileID = LinePoints[i];
		auto LastTileID = LinePoints[i - 1];
		FVector CurrentTileCoords, LastTileCorrds;
		CurrentTileCoords.X = CurrentTileID % 6;
		CurrentTileCoords.Y = CurrentTileID / 6;
		LastTileCorrds.X = LastTileID % 6;
		LastTileCorrds.Y = LastTileID / 6;

		UPaperSpriteComponent* NewLineSegmentSprite = nullptr;

		// Caculate the new line body sprite rotation angle
		int32 NewSpriteAngle = 0;
		if (CurrentTileCoords.X - LastTileCorrds.X == 1)
		{
			if (CurrentTileCoords.Y == LastTileCorrds.Y)
			{
				NewSpriteAngle = 0;
			}
			else if (CurrentTileCoords.Y - LastTileCorrds.Y == 1)
			{
				NewSpriteAngle = 45;
			}
			else if (CurrentTileCoords.Y - LastTileCorrds.Y == -1)
			{
				NewSpriteAngle = 315;
			}
		}
		else if (CurrentTileCoords.X - LastTileCorrds.X == 0)
		{
			if (CurrentTileCoords.Y - LastTileCorrds.Y == 1)
			{
				NewSpriteAngle = 90;
			}
			if (CurrentTileCoords.Y - LastTileCorrds.Y == -1)
			{
				NewSpriteAngle = 270;
			}
		}
		else if (CurrentTileCoords.X - LastTileCorrds.X == -1)
		{
			if (CurrentTileCoords.Y == LastTileCorrds.Y)
			{
				NewSpriteAngle = 180;
			}
			else if (CurrentTileCoords.Y - LastTileCorrds.Y == 1)
			{
				NewSpriteAngle = 135;
			}
			else if (CurrentTileCoords.Y - LastTileCorrds.Y == -1)
			{
				NewSpriteAngle = 225;
			}
		}

		// Create line corners
		if (i >= 2)
		{
			// Check if the two line in the same direction (positive or negative), if so no corner needed
			if ((NewSpriteAngle + 360 - m_LastAngle) % 180 != 0)
			{
				UPaperSpriteComponent* NewLineCornerSprite = nullptr;
				NewLineCornerSprite = CreateLineCorner(NewSpriteAngle, m_LastAngle);
				if (NewLineCornerSprite == nullptr)
				{
					UE_LOG(LogSGame, Warning, TEXT("New corner sprite create failed."));
					return false;
				}

				// Set to the last point location
				FVector CornerPosition;
				CornerPosition.X = (LastTileCorrds.X - InitialTileCorrds.X) * 70;

				// We want the corner sort infront of lines to 
				// make the intersection more beautiful
				CornerPosition.Y = 10;
				CornerPosition.Z = (LastTileCorrds.Y - InitialTileCorrds.Y) * 70;
				NewLineCornerSprite->SetRelativeLocation(CornerPosition);
			}
		}

		// For the line head
		if (i == LinePoints.Num() - 1)
		{
			// Set to the current point location
			FVector HeadPosition;
			HeadPosition.X = (CurrentTileCoords.X - InitialTileCorrds.X) * 70;
			// We want the head sort infront of lines to 
			// make the intersection more beautiful
			HeadPosition.Y = 10;
			HeadPosition.Z = (CurrentTileCoords.Y - InitialTileCorrds.Y) * 70;
			HeadSpriteRenderComponent->SetRelativeLocation(HeadPosition);

			// Set the head rotation
			HeadSpriteRenderComponent->SetRelativeRotation(FRotator(NewSpriteAngle, 0, 0));
		}

		// Create the line segment
		NewLineSegmentSprite = CreateLineSegment(NewSpriteAngle, i == LinePoints.Num() - 1, i == 1);
		if (NewLineSegmentSprite == NULL)
		{
			UE_LOG(LogSGame, Warning, TEXT("New sprite create failed."));
			return false;
		}

		// Set to the last point location
		FVector LineSegmentPosition;
		LineSegmentPosition.X = (LastTileCorrds.X - InitialTileCorrds.X) * 70;
		LineSegmentPosition.Y = i == 1 ? -10 : 0;
		LineSegmentPosition.Z = (LastTileCorrds.Y - InitialTileCorrds.Y) * 70;
		NewLineSegmentSprite->SetRelativeLocation(LineSegmentPosition);

		// Mark down current angle
		m_LastAngle = NewSpriteAngle;
	}

	return true;
}

bool ASGLinkLine::Update()
{
	UpdateLinkLineSprites(LinkLinePoints);

	return true;
}

#if WITH_EDITOR
bool ASGLinkLine::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);

	if (HeadSpriteRenderComponent != nullptr && HeadSpriteRenderComponent->GetSprite() != nullptr)
	{
		Objects.Add(HeadSpriteRenderComponent->GetSprite());
	}

	if (TailSpriteRenderComponent != nullptr && TailSpriteRenderComponent->GetSprite() != nullptr)
	{
		Objects.Add(TailSpriteRenderComponent->GetSprite());
	}

	for (int i = 0; i < LinkLineSpriteRendererArray.Num(); i++)
	{
		if (LinkLineSpriteRendererArray[i] != nullptr && LinkLineSpriteRendererArray[i]->GetSprite() != nullptr)
		{
			Objects.Add(LinkLineSpriteRendererArray[i]->GetSprite());
		}
	}
	return true;
}
#endif

bool ASGLinkLine::ContainsTileAddress(int32 inTileAddress)
{
	return LinkLinePoints.Contains(inTileAddress);
}

bool ASGLinkLine::ReplayLinkAniamtion(TArray<ASGTileBase*>& CollectTiles)
{
	CachedCollectTiles = CollectTiles;

	if (MessageEndpoint.IsValid() == true)
	{
		// Reset the tile link status
		FMessage_Gameplay_TileLinkedStatusChange* LinkStatusChangeMessage = new FMessage_Gameplay_TileLinkedStatusChange{ 0 };
		LinkStatusChangeMessage->TileID = -1;
		LinkStatusChangeMessage->NewLinkStatus = false;
		MessageEndpoint->Publish(LinkStatusChangeMessage, EMessageScope::Process);

		// Reset the tile selectable status
		FMessage_Gameplay_TileSelectableStatusChange* SelectableMessage = new FMessage_Gameplay_TileSelectableStatusChange{ 0 };
		SelectableMessage->TileID = -1;
		SelectableMessage->NewSelectableStatus = true;
		MessageEndpoint->Publish(SelectableMessage, EMessageScope::Process);
	}

	// Kick off the replay
	BeginReplayLinkAnimation();

	return true;
}

void ASGLinkLine::EndReplayLinkAnimation()
{
	UE_LOG(LogSGame, Log, TEXT("End replay link line anim."));

	ASGGameMode* GameMode = Cast<ASGGameMode>(UGameplayStatics::GetGameMode(this));
	checkSlow(GameMode);

	IsReplayingLinkLineAnimation = false;

	// Finally collect the tile resources
	if (CachedCollectTiles.Num() > 0)
	{
		checkSlow(ParentGrid);
		GameMode->CollectTileArray(CachedCollectTiles);
	}
	else
	{
		// We don't need to refill the grid, send tile finish moving message directly
		checkSlow(MessageEndpoint.IsValid());
		FMessage_Gameplay_AllTileFinishMove* Message = new FMessage_Gameplay_AllTileFinishMove();
		MessageEndpoint->Publish(Message, EMessageScope::Process);
	}
	
	// Reset the linkline after all
	ResetLinkState();
}

void ASGLinkLine::TickReplayLinkHeadAnimation(float DeltaSeconds)
{
	if (IsReplayingLinkLineAnimation == false)
	{
		return;
	}

	float Ratio = ReplayHeadAnimationElapsedTime / ReplayHeadAnimationDuration;
	if (Ratio > 1.0f)
	{
		// Current segement replay is finished
		IsReplayingLinkLineAnimation = false;
		return;
	}
	else
	{
		if (LinkLineSpriteRendererArray.Num() > 0)
		{
			// Get the last body sprite
			UPaperSpriteComponent* LastLinebody = LinkLineSpriteRendererArray.Last();
			checkSlow(LastLinebody);
			LastLinebody->SetRelativeScale3D(FVector(Ratio + 0.2f, 1.0f, 1.0f));
		}
		else
		{
			// Set the tail sprite scale
			checkSlow(TailSpriteRenderComponent);
			TailSpriteRenderComponent->SetRelativeScale3D(FVector(Ratio + 0.2f, 1.0, 1.0f));
		}
	}

	ReplayHeadAnimationElapsedTime += DeltaSeconds;
}

void ASGLinkLine::ReplaySingleLinkLineAniamtion(int32 ReplayLength)
{
	UE_LOG(LogSGameAsyncTask, Log, TEXT("Starting replay link line anim with length: %d"), ReplayLength);
	TArray<int32> ReplayingLinkLinePoints;

	// Reconstruct the replay link line points one by one
	ReplayingLinkLinePoints.Empty();

	// Push back the replayed length
	for (int i = 0; i <= ReplayLength; i++)
	{
		ReplayingLinkLinePoints.Push(LinkLinePoints[i]);
	}

	// Construct the link line again
	UpdateLinkLineSprites(ReplayingLinkLinePoints);

	checkSlow(ParentGrid);
	if (this->MessageEndpoint.IsValid() == true)
	{
		if (ReplayLength == 1)
		{
			// Send the selected message to the fake tail
			FMessage_Gameplay_TileLinkedStatusChange* LinkStatusChangeMessage = new FMessage_Gameplay_TileLinkedStatusChange{ 0 };
			const ASGTileBase* FakeSelectedTile = ParentGrid->GetTileFromGridAddress(LinkLinePoints[0]);
			LinkStatusChangeMessage->TileID = FakeSelectedTile->GetTileID();
			LinkStatusChangeMessage->NewLinkStatus = true;
			MessageEndpoint->Publish(LinkStatusChangeMessage, EMessageScope::Process);

			// If the tile is an enemy tile, then play hit animation
			FMessage_Gameplay_EnemyGetHit* HitMessage = new FMessage_Gameplay_EnemyGetHit{ 0 };
			HitMessage->TileID = FakeSelectedTile->GetTileID();
			MessageEndpoint->Publish(HitMessage, EMessageScope::Process);
		}

		// Send the selected message to the fake head
		FMessage_Gameplay_TileLinkedStatusChange* LinkStatusChangeMessage = new FMessage_Gameplay_TileLinkedStatusChange{ 0 };
		const ASGTileBase* FakeSelectedTile = ParentGrid->GetTileFromGridAddress(LinkLinePoints[ReplayLength]);
		LinkStatusChangeMessage->TileID = FakeSelectedTile->GetTileID();
		LinkStatusChangeMessage->NewLinkStatus = true;
		MessageEndpoint->Publish(LinkStatusChangeMessage, EMessageScope::Process);

		// If the tile is an enemy tile, then play hit animation
		FMessage_Gameplay_EnemyGetHit* HitMessage = new FMessage_Gameplay_EnemyGetHit{ 0 };
		HitMessage->TileID = FakeSelectedTile->GetTileID();
		MessageEndpoint->Publish(HitMessage, EMessageScope::Process);
	}

	// Replay head anim
	ReplayLinkHeadAnimation();
}

UPaperSpriteComponent* ASGLinkLine::CreateLineCorner(int inAngle, int inLastAngle)
{
	// Create the base sprite and initialize
	UPaperSpriteComponent* NewSprite = nullptr;
	NewSprite = NewObject<UPaperSpriteComponent>(this);
	NewSprite->Mobility = EComponentMobility::Movable;
	NewSprite->RegisterComponent();
	NewSprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	NewSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Rotate to the last angle
	NewSprite->SetRelativeRotation(FRotator(inLastAngle, 0, 0));
	
	// Choose the sprite texture
	int32 AngleDiff = (inAngle - inLastAngle + 360) % 360;
	switch (AngleDiff)
	{
	case 45:
	{
		NewSprite->SetRelativeScale3D(FVector(1, 1, -1));
		NewSprite->SetSprite(Corner_135_Sprite);
		break;
	}
	case 225:
	{
		NewSprite->SetSprite(Corner_45_Sprite);
		break;
	}
	case 90:
	{
		NewSprite->SetRelativeScale3D(FVector(1, 1, -1));
		NewSprite->SetSprite(Corner_90_Sprite);
		break;
	}
	case 270:
	{
		NewSprite->SetSprite(Corner_90_Sprite);
		break;
	}
	case 135:
	{
		NewSprite->SetRelativeScale3D(FVector(1, 1, -1));
		NewSprite->SetSprite(Corner_45_Sprite);
		break;
	}
	case 315:
	{
		NewSprite->SetSprite(Corner_135_Sprite);
		break;
	}
	default:
	{
		UE_LOG(LogSGame, Warning, TEXT("Invalid angle, returning null sprite!"));
		return nullptr;
	}
	}

	// Add to the body sprite array
	LinkLineSpriteRendererArray.Add(NewSprite);

	return NewSprite;
}

UPaperSpriteComponent* ASGLinkLine::CreateLineSegment(int inAngle, bool inIsHead, bool inIsTail)
{
	UPaperSpriteComponent* NewSprite = NULL;
	if (inIsTail == true)
	{
		NewSprite = TailSpriteRenderComponent;
	}
	else
	{
		NewSprite = NewObject<UPaperSpriteComponent>(this);
		NewSprite->Mobility = EComponentMobility::Movable;
		NewSprite->SetSprite(BodySprite);
		NewSprite->RegisterComponent();
		NewSprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NewSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Add to the body sprite array
		LinkLineSpriteRendererArray.Add(NewSprite);
	}

	checkSlow(NewSprite != nullptr);

	if (inAngle % 90 != 0)
	{
		// Set Scale to 1.414 if it is cross line
		NewSprite->SetRelativeScale3D(FVector(1.42f, 1, 1));
	}
	else
	{
		// Set Scale to a little bit longer than 1 to overlap
		NewSprite->SetRelativeScale3D(FVector(1.05f, 1, 1));
	}

	// Set the rotation to the new angle
	NewSprite->SetRelativeRotation(FRotator(inAngle, 0, 0));

	return NewSprite;
}

void ASGLinkLine::ReplayLinkHeadAnimation()
{
	// Reset the played link line percentage
	ReplayHeadAnimationElapsedTime = 0;

	// Turn on to use body lerp animation
	IsReplayingLinkLineAnimation = false;
}

void ASGLinkLine::ResetLinkState()
{
	// Cleaer the current link
	LinkLineTiles.Empty();
	LinkLinePoints.Empty();

	// Update the link line
	Update();
}

void ASGLinkLine::BuildPath(ASGTileBase* inNewTile)
{
	checkSlow(inNewTile);
	UE_LOG(LogSGame, Log, TEXT("Linkline build path using tile %d"), inNewTile->GetGridAddress());

	if (inNewTile->IsSelectable() == false)
	{
		// Current tile cannot be as the head of the link line
		return;
	}

	// Check if the new address already exists in the points array
	int32 ExistTileAddress = INDEX_NONE;
	if (LinkLineTiles.Find(inNewTile, ExistTileAddress) == true)
	{
		// Pop out all the points after the address
		for (int i = LinkLinePoints.Num() - 1; i > ExistTileAddress; i--)
		{
			LinkLinePoints.Pop();
			LinkLineTiles.Pop();
		}
	}
	else
	{
		// Add the tile to the link line
		LinkLineTiles.Add(inNewTile);

		// Add the points to the link line points for drawing the sprites
		LinkLinePoints.Add(inNewTile->GetGridAddress());
	}

	// Do a link line update
	Update();
}

