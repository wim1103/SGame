// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SGTileBase.h"
#include "SGGlobalGameInstance.h"

#include "SGEnemyTileBase.generated.h"

/**
 * Enemy base tile
 */
UCLASS()
class SGAME_API ASGEnemyTileBase : public ASGTileBase
{
	GENERATED_BODY()

	friend class USGCheatManager;

public:
	ASGEnemyTileBase();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// The sprite asset for attcking state
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Sprite_Attacking;

	// The sprite asset for dead state
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Sprite_Dead;

	// The attack text render component
	UPROPERTY(Category = Text, EditAnywhere, BlueprintReadOnly)
	UTextRenderComponent* Text_Attack;

	// The armor text render component
	UPROPERTY(Category = Text, EditAnywhere, BlueprintReadOnly)
	UTextRenderComponent* Text_Armor;

	// The hp text render component
	UPROPERTY(Category = Text, EditAnywhere, BlueprintReadOnly)
	UTextRenderComponent* Text_HP;

	/** Tick attacking */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void TickAttacking(float DeltaSeconds);

	/** Begin attack */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void EnemyAttack();

	// Start Attack, using BP function to implement, since it is more convenient to polish
	UFUNCTION(BlueprintImplementableEvent)
	void StartAttack();

	/** End attack */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void EndAttack();

	/** Tick playing hit */
	UFUNCTION(BlueprintCallable, Category = Hit)
	void TickPlayHit(float DeltaSeconds);

	/** Begin play hit */
	UFUNCTION(BlueprintCallable, Category = Hit)
	void BeginPlayHit();

	/** End play hit */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void EndPlayHit();

	bool	bIsAttacking;
	bool	bIsPlayingHit;

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

	/** How long will it takes to finish a attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	float PlayHitDuration;
	float PlayHitElapsedTime;

private:
	// Holds the messaging endpoint.
	// Noted that this class may have two message endpoint, 
	// one for its parent messages and handlers, and 
	// one for itself
	FMessageEndpointPtr MessageEndpoint;

	/** Handle begin attack message */
	void HandleBeginAttack(const FMessage_Gameplay_EnemyBeginAttack& Message, const IMessageContextRef& Context);

	/** Handle play hit animation and effects */
	void HandlePlayHit(const FMessage_Gameplay_EnemyGetHit& Message, const IMessageContextRef& Context);
};
