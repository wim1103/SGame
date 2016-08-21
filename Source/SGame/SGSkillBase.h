// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "SGSkillBase.generated.h"

USTRUCT()
struct FSGSkillBaseData
{
	GENERATED_USTRUCT_BODY();

	/** The default CD for the skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FString SkillName;

	/** The default CD for the skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	int32 DefaultCD;
};

UCLASS()
class SGAME_API ASGSkillBase : public AActor
{
	friend class USGPlayerSkillManager;

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGSkillBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Condense the grid, fill the holes*/
	UFUNCTION(BlueprintCallable, Category = Skill)
	bool IsSkillReady()
	{
		return (RemainingCD <= 0);
	}

	/** blueprint event: pickup appears */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerUseSkill();

	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	/** Returns RenderComponent subobject **/
	FORCEINLINE class UPaperSpriteComponent* GetRenderComponent() const { return RenderComponent; }

protected:

	/** Whether the enemy can take damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FSGSkillBaseData BaseSkillInfo;

	/** Current CD for the skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	int32 CurrentCD;

	/** Remaining CD for the skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	int32 RemainingCD;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* RenderComponent;
};
