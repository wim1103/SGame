// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "iTweenPCH.h"
#include "iTSpline.generated.h"

/**
 * 
 */
UCLASS()
class SGAME_API AiTSpline : public AActor
{
	GENERATED_BODY()

	AiTSpline(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* spline; 
};
