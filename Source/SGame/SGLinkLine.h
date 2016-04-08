// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

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

	/** Update Link Line */
	UFUNCTION(BlueprintCallable, Category = Update)
	bool UpdateLinkLine();

	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	/** Whether it is a static line. Test only, for static link lines.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStaticLine;

	/** Static line points. Test only, for static link lines.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> StaticLinePoints;

	/** Link line points */
	TArray<int32> LinkLinePoints;

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
	TArray<UPaperSpriteComponent*> BodySpriteRenderComponentsArray;

	/** Update link line using the line points */
	bool UpdateLinkLine(const TArray<int32>& LinePoints);

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
public:
	void ResetLinkState();
};
