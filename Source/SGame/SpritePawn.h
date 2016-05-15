// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteComponent.h"
#include "GameFramework/Pawn.h"
#include "SpritePawn.generated.h"

UCLASS()
class SGAME_API ASpritePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpritePawn();

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

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SetHealth", Category = "Health"))
	void SetCurrentHealth(int inNewHealth);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		HPMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		CurrentArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	int32		ArmorMax;

private:
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* RenderComponent;
};
