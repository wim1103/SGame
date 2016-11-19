// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Messaging.h"

#include "SGameMessages.h"
#include "SGTileBase.h"

#include "SGLinkLine.generated.h"

UCLASS()
class SGAME_API ASGLinkLine : public AActor
{
	GENERATED_BODY()

	enum ELinkDirection
	{
		ELD_Begin,
		ELD_Horizon,
		ELD_Vertical,
		ELD_BackSlash,
		ELD_Slash,
	};
	
public:	
	// Sets default values for this actor's properties
	ASGLinkLine();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Update link line sprites*/
	UFUNCTION(BlueprintCallable, Category = Update)
	bool UpdateLinkLineSprites();

	/** Update the link line*/
	UFUNCTION(BlueprintCallable, Category = Update)
	bool Update();

	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	/** Return whether the current link line contains the tile*/
	UFUNCTION(BlueprintCallable, Category = Visitor)
	bool ContainsTileAddress(int32 inTileAddress);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	ReplayHeadAnimationDuration;
	float	ReplayHeadAnimationElapsedTime;

	/** 
	 * Replay link animation 
	 *
	 * @return true to replay successfully
	 */
	UFUNCTION(BlueprintCallable, Category = Visitor)
	bool ReplayLinkAniamtion(TArray<ASGTileBase*>& CollectTiles);
	bool IsReplayingLinkLineAnimation;

	/**
	* Replay link animation
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void BeginReplayLinkAnimation();

	/**/
	UFUNCTION(BlueprintCallable, Category = Visitor)
	void EndReplayLinkAnimation();
	void TickReplayLinkHeadAnimation(float DeltaSeconds);

	/**
	* Replay single unit linkline animation
	*
	* @param ReplayLength CurrentReplayLength
	*/
	UFUNCTION(BlueprintCallable, Category = Visitor)
	void ReplaySingleLinkLineAniamtion(int32 ReplayLength);

	/** Whether it is a static line. Test only, for static link lines.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStaticLine;

	/** Static line points. Test only, for static link lines.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> StaticLinePoints;

	/** Current tiles in the link line*/
	TArray<ASGTileBase*> LinkLineTiles;

protected:
	// The sprite asset for link corners 45 degree
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Corner_45_Sprite;

	// The sprite asset for link corners 90 degree
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Corner_90_Sprite;

	// The sprite asset for link corners 135 degree
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Corner_135_Sprite;

	// The sprite asset for link body
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* BodySprite;

	/** Body sprites for render the link line body lines and coners */
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly)
	TArray<UPaperSpriteComponent*> LinkLineSpriteRendererArray;

	/** Update link line using the line points */
	bool UpdateLinkLineSprites(const TArray<int32>& LinePoints);

	/** Link line points, for drawing the sprites*/
	UPROPERTY(Category = LinePoints, VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> LinkLinePoints;

private:
	/** Head sprite for render the link line head */
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	UPaperSpriteComponent* HeadSpriteRenderComponent;

	/** Tail sprite for render the link line tail */
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	UPaperSpriteComponent* TailSpriteRenderComponent;

	UPaperSpriteComponent* CreateLineCorner(int inAngle, int inLastAngle);
	UPaperSpriteComponent* CreateLineSegment(int inAngle, bool inIsHead, bool inIsTail);

	int								m_CurrentSpriteNum;
	int								m_LastAngle;

	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;

	// Hold the reference to its parent grid
	ASGGrid* ParentGrid;

	/** Cached current turn collected tiles for do collect animation after replay link line animation */
	TArray<ASGTileBase*> CachedCollectTiles;

	void ReplayLinkHeadAnimation();
public:
	void ResetLinkState();
	void BuildPath(ASGTileBase* inNewTile);

};
