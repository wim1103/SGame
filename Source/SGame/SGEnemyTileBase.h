// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SGTileBase.h"
#include "SGEnemyTileBase.generated.h"

/**
 * 
 */
UCLASS()
class SGAME_API ASGEnemyTileBase : public ASGTileBase
{
	GENERATED_BODY()

public:
	ASGEnemyTileBase();

	/** Tick attacking */
	UFUNCTION()
	void TickAttacking(float DeltaSeconds);

	/** Tick attacking */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BeginAttack();

	/** Tick attacking */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void EndAttack();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

protected:

	// The sprite asset for selected state
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Sprite_Attacking;

	bool	bIsAttacking;

	/** How long will it takes to finish a attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackingDuration;
	float AttackingElapsedTime;

	// Divide the animation to sub sequences and write them by code!

	/** Time window to scale up or down 
	[0, AttackingScaleTimeWindow] -> Up 
	[1 - AttackingScaleTimeWindow, 1] -> Down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackingScaleTimeWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackingScaleRatio;

	// Shaking stage
	/** Degree to rotate to simulate the shake effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackingShakeDegree;

	/** Frequency to rotate to simulate the shake effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 AttackingShakeFreq;
};
