// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "iTweenPCH.h"
#include "UMG.h"
#include "iTAux.h"
#include "iTInterface.generated.h"

/**
*
*/
using namespace ELoopType;
UINTERFACE()
class SGAME_API UiTInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IiTInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Start"), Category = "iTween|Interface Events")
		void OnTweenStart(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "");

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Update"), Category = "iTween|Interface Events")
		void OnTweenUpdate(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FDataTypeValues dataTypeValues, float alphaCompletion = 0.f);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Tick", DeprecatedFunction, DeprecationMessage = "Interface Event deprecated, use On Tween Update instead."), Category = "iTween|Interface Events")
		void OnTweenTick(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "", float alphaCompletion = 0.f);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Data Tick", DeprecatedFunction, DeprecationMessage = "Interface Event deprecated, use On Tween Update instead. Data values are now stored in 'Data Type Values' struct. Break that to get float value, vector value, etc."), Category = "iTween|Interface Events")
		void OnTweenDataTick(AiTweenEvent* eventOperator, FName tweenName = "", float floatValue = 0.f, FLinearColor linearColorValue = FLinearColor::Black, FRotator rotatorValue = FRotator::ZeroRotator, FVector vectorValue = FVector::ZeroVector, FVector2D vector2DValue = FVector2D::ZeroVector, float alphaCompletion = 0.f);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Loop"), Category = "iTween|Interface Events")
		void OnTweenLoop(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "", int32 numberOfLoopSections = 0, ELoopType::LoopType loopType = once, bool playingBackward = false);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "On Tween Complete"), Category = "iTween|Interface Events")
		void OnTweenComplete(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform);

	virtual void OnTweenStartNative(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "");

	virtual void OnTweenUpdateNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FDataTypeValues dataTypeValues, float alphaCompletion = 0.f);

	virtual void OnTweenLoopNative(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "", int32 numberOfLoopSections = 0, ELoopType::LoopType loopType = once, bool playingBackward = false);

	virtual void OnTweenCompleteNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform);

};

class STweenableWidget : public SCompoundWidget
{
public:
	virtual void OnTweenStart(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "");

	virtual void OnTweenUpdate(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FDataTypeValues dataTypeValues, float alphaCompletion = 0.f);

	virtual void OnTweenTick(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "", float alphaCompletion = 0.f);

	virtual void OnTweenDataTick(AiTweenEvent* eventOperator, FName tweenName = "", float floatValue = 0.f, FLinearColor linearColorValue = FLinearColor::Black, FRotator rotatorValue = FRotator::ZeroRotator, FVector vectorValue = FVector::ZeroVector, FVector2D vector2DValue = FVector2D::ZeroVector, float alphaCompletion = 0.f);

	virtual void OnTweenLoop(AiTweenEvent* eventOperator, AActor* actorTweening = nullptr, USceneComponent* componentTweening = nullptr, UWidget* widgetTweening = nullptr, FName tweenName = "", int32 numberOfLoopSections = 0, ELoopType::LoopType loopType = once, bool playingBackward = false);

	virtual void OnTweenComplete(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform);

	void SetTweenScale(FVector2D Scale);

	void SetTweenShear(FVector2D Shear);

	void SetTweenAngle(float Angle);

	void SetTweenTranslation(FVector2D Translation);

	void UpdateRenderTransform();

	FWidgetTransform TweenTransform;

};
