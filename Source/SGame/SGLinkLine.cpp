// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGLinkLine.h"
#include "UObjectGlobals.h"

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

	if (bIsStaticLine == true)
	{
	}
}

// Called every frame
void ASGLinkLine::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool ASGLinkLine::UpdateLinkLine()
{
	return true;
	/*
	if (bIsStaticLine == true)
	{
		if (StaticLinePoints.Num() < 2)
		{
			return true;
		}

		// Clean the body sprites
		for (int32 i = 0; i < BodySpriteRenderComponentsArray.Num(); i++)
		{
			checkSlow(BodySpriteRenderComponentsArray[i] != nullptr);
			// Detach the component
			BodySpriteRenderComponentsArray[i]->DetachFromParent();
			BodySpriteRenderComponentsArray[i]->UnregisterComponent();
		}
		BodySpriteRenderComponentsArray.Empty();

		// Iterate all points, and generate line between the two points
		ELinkDirection LastDirection = ELD_Begin;
		for (int32 i = 1; i < StaticLinePoints.Num(); i++)
		{
			auto CurrentTileID = StaticLinePoints[i];
			auto LastTileID = StaticLinePoints[i - 1];

			FVector CurrentTileCoords, LastTileCorrds;
			CurrentTileCoords.X = CurrentTileID / 6;
			CurrentTileCoords.Y = CurrentTileID % 6;
			LastTileCorrds.X = LastTileID / 6;
			LastTileCorrds.Y = LastTileID % 6;

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

					// 将Position设置到上一个的中心
					FVector LinePosition;
					LinePosition.X = LastTileCorrds.X * 106 + 106 / 2;
					LinePosition.Z = (6 - LastTileCorrds.Y - 0.5f) * 106;
					NewLineCornerSprite->SetRelativeLocation(LinePosition);
				}
			}

			// 重定位线段头
			if (i == stPath.GetLength() - 1)
			{
				// 将Position设置到上一个的中心
				CCPoint LinePosition;
				LinePosition.x = CurrentTileCoords.X * GRID_COLUMN_SIZE + GRID_COLUMN_SIZE / 2;
				LinePosition.y = (MAX_FIELD_ROW_NUM - CurrentTileCoords.Y - 0.5f) * GRID_ROW_SIZE;
				m_pArrowHead->setPosition(LinePosition);
				m_pArrowHead->setRotation(NewSpriteAngle);
			}

			// 创建线段
			NewLineSegmentSprite = CreateLineSegment(NewSpriteAngle, i == stPath.GetLength() - 1, i == 1);
			if (NewLineSegmentSprite == NULL)
			{
				LOG_ERR("New sprite create failed.");
				return false;
			}

			// 将Position设置到上一个的中心
			CCPoint LinePosition;
			LinePosition.x = LastTileCorrds.X * GRID_COLUMN_SIZE + GRID_COLUMN_SIZE / 2;
			LinePosition.y = (MAX_FIELD_ROW_NUM - LastTileCorrds.Y - 0.5f) * GRID_ROW_SIZE;
			NewLineSegmentSprite->setPosition(LinePosition);

			// 添加到Batch Sprite里
			m_pSpriteBatchNode->addChild(NewLineSegmentSprite, 100);

			// 记录当前的Angle
			m_LastAngle = NewSpriteAngle;
		}
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
	NewSprite->AttachParent = RootComponent;
	BodySpriteRenderComponentsArray.Add(NewSprite);
	
	// 1. We should set the proper sprite based on the angle
	// 2. We should do some flip adjustment based on the angle
	if (abs(inAngle - inLastAngle) == 45)
	{
		NewSprite->SetSprite(Corner_135_Sprite);
		if (inAngle < inLastAngle)
		{
			// FlipY
			NewSprite->SetRelativeScale3D(FVector(1, -1, 1));
		}
	}
	else if (abs(inAngle - inLastAngle) == 90)
	{
		NewSprite->SetSprite(Corner_90_Sprite);
		if (inAngle < inLastAngle)
		{
			// FlipY
			NewSprite->SetRelativeScale3D(FVector(1, -1, 1));
		}
	}
	else if (abs(inAngle - inLastAngle) == 135)
	{
		NewSprite->SetSprite(Corner_45_Sprite);
		if (inAngle < inLastAngle)
		{
			// FlipY
			NewSprite->SetRelativeScale3D(FVector(1, -1, 1));
		}
	}
	/*
	else if (abs(inAngle - inLastAngle) == 225)
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_45_FRAME_NAME);
		NewSprite->setFlipY(inAngle > inLastAngle);
	}

	
	if (abs(inAngle - inLastAngle) == 45)
	{
		NewSprite->
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_135_FRAME_NAME);
		NewSprite->setFlipY(inAngle < inLastAngle);
	}
	else if (abs(inAngle - inLastAngle) == 90)
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_90_FRAME_NAME);
		NewSprite->setFlipY(inAngle < inLastAngle);
	}
	else if (abs(inAngle - inLastAngle) == 135)
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_45_FRAME_NAME);
		NewSprite->setFlipY(inAngle < inLastAngle);
	}
	else if (abs(inAngle - inLastAngle) == 225)
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_45_FRAME_NAME);
		NewSprite->setFlipY(inAngle > inLastAngle);
	}
	else if (abs(inAngle - inLastAngle) == 270)
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_90_FRAME_NAME);
		NewSprite->setFlipY(inAngle > inLastAngle);
	}
	else if (abs(inAngle - inLastAngle) == 315)
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_135_FRAME_NAME);
		NewSprite->setFlipY(inAngle > inLastAngle);
	}

	// 使用上一次的旋转作为旋转
	NewSprite->setRotation(inLastAngle);

	// 设置缩放
	NewSprite->setScale(ARROW_THICKNESS);

	// 保存当前的Sprite
	m_pCurrentArrowSprites[m_CurrentSpriteNum++] = NewSprite;
	*/
	return NewSprite;
}

UPaperSpriteComponent* ASGLinkLine::CreateLineSegment(int inAngle, bool inIsHead, bool inIsTail)
{
	return nullptr;
	/*
	CCSprite* NewSprite = NULL;
	if (inIsTail == true)
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_TAIL_FRAME_NAME);
	}
	else
	{
		NewSprite = CCSprite::createWithSpriteFrameName(Link_Line_BODY_FRAME_NAME);
	}

	if (NewSprite == NULL)
	{
		LOG("Cannot create new arrow body sprite");
		return NULL;
	}

	// 设置在左侧居中，因为好对位
	NewSprite->setAnchorPoint(CCPointMake(0, 0.5f));

	// 设置Scale
	if (inAngle % 90 == 0)
	{
		// 直线就直接是Griz Size
		NewSprite->setScaleX(GRID_COLUMN_SIZE / NewSprite->getContentSize().width);
		NewSprite->setScaleY(ARROW_THICKNESS);
	}
	else
	{
		// 对角线是根号2
		NewSprite->setScaleX(GRID_COLUMN_SIZE * 1.42 / NewSprite->getContentSize().width);
		NewSprite->setScaleY(ARROW_THICKNESS);
	}

	// 设置旋转
	NewSprite->setRotation(inAngle);

	// 保存当前的Sprite
	m_pCurrentArrowSprites[m_CurrentSpriteNum++] = NewSprite;

	return NewSprite;
	*/
}

#endif
