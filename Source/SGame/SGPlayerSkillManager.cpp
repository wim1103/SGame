// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGPlayerSkillManager.h"

ASGSkillBase* USGPlayerSkillManager::CreateSkillByName(const AActor* inOwner, FName inSkillName)
{
	checkSlow(inOwner);

	TSubclassOf<class ASGSkillBase> TargetSkillClass;
	for (const FSGPlayerSkillType& PlayerSkill: PlayerSkillLibrary)
	{
		if (PlayerSkill.bOverrideBaseSkillConfig == true)
		{
			// If override the default skill config, check with the override skill name
			if (PlayerSkill.OverrideSkillInfo.SkillName == inSkillName)
			{
				TargetSkillClass = PlayerSkill.SkillClass;
				break;
			}
		}
		else
		{
			// Otherwise check with the skill default object's name
			const ASGSkillBase* DefaultSkillObject = TargetSkillClass.GetDefaultObject();
			
			checkSlow(DefaultSkillObject);
			if (DefaultSkillObject == nullptr)
			{
				UE_LOG(LogSGame, Error, TEXT("Null class in the skill library, return"));
				continue;
			}
			else
			{
				if (DefaultSkillObject->BaseSkillInfo.SkillName == inSkillName)
				{
					TargetSkillClass = PlayerSkill.SkillClass;
					break;
				}
			}
		}
	}

	if (TargetSkillClass->GetDefaultObject() == NULL)
	{
		UE_LOG(LogSGame, Warning,  TEXT("Player skill library don't find the target name skill %s, return"), *inSkillName.ToString());
		return nullptr;
	}

	// Check for a valid World:
	UWorld* const World = inOwner->GetWorld();
	if (World)
	{
		// Spawn the skill actor.
		ASGSkillBase* const NewSkillInstance = World->SpawnActor<ASGSkillBase>(TargetSkillClass);
		if (NewSkillInstance == nullptr)
		{
			UE_LOG(LogSGame, Warning, TEXT("Player skill created failed, return null ptr"), *inSkillName.ToString());
			return nullptr;
		}

		return NewSkillInstance;
	}

	return nullptr;
}
