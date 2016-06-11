// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SGSkillBase.h"

#include "SGPlayerSkillManager.generated.h"

/**
 * Every player skill should be setup here, and indexed by the skill library
 */
USTRUCT(BlueprintType)
struct FSGPlayerSkillType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TSubclassOf<class ASGSkillBase> SkillClass;

	UPROPERTY(EditAnywhere, Category = Skill)
	bool bOverrideBaseSkillConfig;

	/** Whether the enemy can take damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FSGSkillBaseData OverrideSkillInfo;
};

/**
 * Manage the player skill
 */
UCLASS()
class SGAME_API USGPlayerSkillManager : public UObject
{
	GENERATED_BODY()

public:
	/** Condense the grid, fill the holes*/
	UFUNCTION(BlueprintCallable, Category = Skill)
	ASGSkillBase* CreateSkillByName(const AActor* inOwner, FName inSkillName);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TArray<FSGPlayerSkillType> PlayerSkillLibrary;
};
