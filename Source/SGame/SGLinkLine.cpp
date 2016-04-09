// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGLinkLine.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASGLinkLine::ASGLinkLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Movable;

	HeadSpriteRenderComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LinkLineSpriteComponent-Head"));
	HeadSpriteRenderComponent->Mobility = EComponentMobility::Movable;
	HeadSpriteRenderComponent->AttachParent = RootComponent;

	TailSpriteRenderComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LinkLineSpriteComponent-Tail"));
	TailSpriteRenderComponent->Mobility = EComponentMobility::Movable;
	TailSpriteRenderComponent->AttachParent = RootComponent;
}

// Called when the game starts or when spawned
void ASGLinkLine::BeginPlay()
{
	Super::BeginPlay();
	
	// Build the link line message endpoint
	MessageEndpoint = FMessageEndpoint::Builder("Gameplay_LinkLine");
}

// Called every frame
void ASGLinkLine::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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
	for (int32 i = 0; i < BodySpriteRenderComponentsArray.Num(); i++)
	{
		checkSlow(BodySpriteRenderComponentsArray[i] != nullptr);
		// Detach the component
		BodySpriteRenderComponentsArray[i]->DetachFromParent();
		BodySpriteRenderComponentsArray[i]->UnregisterComponent();
	}
	BodySpriteRenderComponentsArray.Empty();

	if (LinkLinePoints.Num() < 2)
	{
		// Only one point cannot become a link line
		return true;
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
				NewSpriteAngle = -45;
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
				NewSpriteAngle = -90;
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
	UpdateTileSelectInfo();
	UpdateLinkLineSprites(LinkLinePoints);

	return true;
}

void ASGLinkLine::UpdateTileSelectInfo()
{
	// If is is the head of the link line
	if (LinkLinePoints.Num() == 0)
	{
		// Tell all the tiles that they can be selected
		if (MessageEndpoint.IsValid() == true)
		{
			FMessage_Gameplay_TileBecomeSelectable* SelectableMessage = new FMessage_Gameplay_TileBecomeSelectable{ 0 };
			SelectableMessage->bFroceAllTileCanBeSelected = true;
			MessageEndpoint->Publish(SelectableMessage, EMessageScope::Process);
		}
	}

	/*
	const CTile* pFirstTile = m_pField->GetTile(m_stPathVec.front());
	if (NULL == pFirstTile)
	{
		LOG_ERR("m_pField->GetTile(%u) returns NULL\n"
			, m_stPathVec.front()
		);
		return;
	}

	const CTile* pLastTile = m_pField->GetTile(m_stPathVec.back());
	if (NULL == pLastTile)
	{
		LOG_ERR("m_pField->GetTile(%u) returns NULL\n"
			, m_stPathVec.back()
		);
		return;
	}


	ENMTILELINKTYPE eLinkType = pFirstTile->GetLinkType();

	for (uint8 uCol = 0; uCol < MAX_FIELD_COL_NUM; ++uCol)
	{
		for (uint8 uRow = 0; uRow < MAX_FIELD_ROW_NUM; ++uRow)
		{
			CTile* pTile = m_pField->GetTile(uCol, uRow);
			if (NULL == pTile)
			{
				continue;
			}

			pTile->ResetSelected();

			if (uCol + 1 >= pLastTile->GetCol()
				&& uCol <= pLastTile->GetCol() + 1
				&& uRow + 1 >= pLastTile->GetRow()
				&& uRow <= pLastTile->GetRow() + 1
				&& (CTileManager::IsLinkableType(eLinkType, pTile->GetLinkType()) || ETT_ARROW == pLastTile->GetType())
				)
			{
				pTile->SetSelectable();
			}
			else
			{
				pTile->ResetSelectable();
			}
		}
	}

	for (Uint16Vec::iterator iter = m_stPathVec.begin()
		; iter != m_stPathVec.end()
		; ++iter
		)
	{
		uint16 uTileID = *iter;
		CTile* pTile = m_pField->GetTile(uTileID);
		if (NULL == pTile)
		{
			assert(0);
			continue;
		}

		pTile->SetSelected();
	}
	*/
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

	for (int i = 0; i < BodySpriteRenderComponentsArray.Num(); i++)
	{
		if (BodySpriteRenderComponentsArray[i] != nullptr && BodySpriteRenderComponentsArray[i]->GetSprite() != nullptr)
		{
			Objects.Add(BodySpriteRenderComponentsArray[i]->GetSprite());
		}
	}
	return true;
}

UPaperSpriteComponent* ASGLinkLine::CreateLineCorner(int inAngle, int inLastAngle)
{
	// Create the base sprite and initialize
	UPaperSpriteComponent* NewSprite = nullptr;
	NewSprite = NewObject<UPaperSpriteComponent>(this);
	NewSprite->Mobility = EComponentMobility::Movable;
	NewSprite->RegisterComponent();
	NewSprite->AttachTo(RootComponent);
	
	int32 AngleDiff = FMath::Abs(inAngle - inLastAngle);
	switch (AngleDiff)
	{
	case 45:
		// Note that, the 135 texture is inner angle, currently we are using the outter angle
		// So we choose 135 sprite
		NewSprite->SetSprite(Corner_135_Sprite);
		break;
	case 90:
		NewSprite->SetSprite(Corner_90_Sprite);
		break;
	case 135:
		// Note that, the 45 texture is inner angle, this we are using the outter angle
		// So we choose 45 sprite
		NewSprite->SetSprite(Corner_45_Sprite);
		break;
	default:
		UE_LOG(LogSGame, Warning, TEXT("Invalid angle, returning null sprite!"));
		return nullptr;
	}

	// Rotate to the last angle
	NewSprite->SetRelativeRotation(FRotator(inLastAngle, 0, 0));

	// Flip the sprite if needed
	if (inLastAngle == 0)
	{
		if (inAngle == AngleDiff)
		{
			NewSprite->SetRelativeScale3D(FVector(1, 1, -1));
		}
	}
	else
	{
		if (inLastAngle < inAngle)
		{
			NewSprite->SetRelativeScale3D(FVector(1, 1, -1));
		}
	}

	// Add to the body sprite array
	BodySpriteRenderComponentsArray.Add(NewSprite);

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
		NewSprite->AttachTo(RootComponent);

		// Add to the body sprite array
		BodySpriteRenderComponentsArray.Add(NewSprite);
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

void ASGLinkLine::ResetLinkState()
{
	// Cleaer the current link
	LinkLinePoints.Empty();

	// Update the link line
	Update();
}

void ASGLinkLine::BuildPath(const ASGTileBase* inNewTile)
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

	// Add the tile to the link line
	LinkLineTiles.Add(inNewTile);

	// Add the points to the link line points for drawing the sprites
	LinkLinePoints.Add(inNewTile->GetGridAddress());

	// Do a link line update
	Update();
}

#endif
