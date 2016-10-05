// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteComponent.h"
#include "GameFramework/Pawn.h"
#include "Messaging.h"
#include "SGameMessages.h"

#include "SGSpritePawn.generated.h"

UCLASS()
class SGAME_API ASGSpritePawn : public APawn
{
	friend class USGCheatManager;

	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASGSpritePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	/** Returns RenderComponent subobject **/
	FORCEINLINE class UPaperSpriteComponent* GetRenderComponent() const { return RenderComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		HPMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		CurrentArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		ArmorMax;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SetHealth", Category = "Health"))
	void SetCurrentHealth(int inNewHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = PlayHit)
	void OnPlayHitAniamtion();

	/** Handles the player picked new tile*/
	void HandlePlayerTakeDamage(const FMessage_Gameplay_PlayerTakeDamage& Message, const IMessageContextRef& Context);

	/** Handles collect resouce*/
	void HandleCollectResouce(const FMessage_Gameplay_ResourceCollect& Message, const IMessageContextRef& Context);

private:
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* RenderComponent;

	// Holds the messaging endpoint.
	FMessageEndpointPtr MessageEndpoint;
};
