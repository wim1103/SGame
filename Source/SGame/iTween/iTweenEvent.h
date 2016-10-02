// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "iTweenPCH.h"
#include "iTAux.h"
#include "iTInterface.h"
#include "iTweenEvent.generated.h"

/**
*
*/
using namespace EVectorConstraints;
using namespace EVector2DConstraints;
using namespace ERotatorConstraints;
using namespace EDelayType;
using namespace ETickType;
using namespace EEaseType;
using namespace ELoopType;
using namespace EEventType;
using namespace ELookType;
using namespace ECoordinateSpace;
using namespace ETweenInterfaceType;

class STweenableWidget;

UCLASS()
class SGAME_API AiTweenEvent : public AActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual	void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:

	//Delegates
	FTimerDynamicDelegate OnTweenStartDelegate;
	FTimerDynamicDelegate OnTweenUpdateDelegate;
	FTimerDynamicDelegate OnTweenLoopDelegate;
	FTimerDynamicDelegate OnTweenCompleteDelegate;

	//Splines
	UPROPERTY()
		USplineComponent* OldSplineComponent = nullptr;

	UPROPERTY()
		bool DestroyNewSplineObject = false;

	virtual void Tick(float DeltaSeconds) override;

	void ExecuteTween();

	void LastSet();

	void EndComponentRotateToSplinePoint();

	void EndActorRotateToSplinePoint();

	void EndComponentMoveToSplinePoint();

	void EndActorMoveToSplinePoint();

	void InterpTween();

	void TickComponentRotateToSplinePoint();

	void TickActorRotateToSplinePoint();

	void TickComponentMoveToSplinePoint();

	void TickActorMoveToSplinePoint();

	void TickRotatorFromTo();

	void TickUMGRTMoveFromTo();

	void TickSlateMoveFromTo();

	void TickComponentRotateFromTo();

	void TickComponentMoveFromTo();

	void TickActorRotateFromTo();

	void FirstSet();

	void TickActorMoveFromTo();

	void OrientationSwitch();

protected:

	AiTweenEvent();

	const float pi = (22 / 7);
	float start = 0.f;
	float end = 1.f;
	float delayCount = 0.f;
	bool shouldTween = false;
	bool firstSet = false;
	bool shouldDelay = false;
	bool successfulTransform = true;
	float deltaSeconds = 0.f;

public:
	//Properties
	//Generic Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true, DisplayName = "iTween Instance"), Category = "Generic Properties")
		AiTAux* aux = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<ECoordinateSpace::CoordinateSpace> coordinateSpace = ECoordinateSpace::CoordinateSpace::world;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool sweep = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool tieToObjectValidity = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool enforceValueTo = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		float timerInterval = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<EVectorConstraints::VectorConstraints> vectorConstraints = EVectorConstraints::VectorConstraints::none;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<EVector2DConstraints::Vector2DConstraints> vector2DConstraints = EVector2DConstraints::Vector2DConstraints::none;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<ERotatorConstraints::RotatorConstraints> rotatorConstraints = ERotatorConstraints::RotatorConstraints::none;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<EDelayType::DelayType> delayType = EDelayType::DelayType::first;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<ETickType::TickType> tickType = ETickType::TickType::seconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<EEaseType::EaseType> easeType = EEaseType::EaseType::linear;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		UCurveFloat* customEaseTypeCurve = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		float punchAmplitude = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<ELoopType::LoopType> loopType = ELoopType::LoopType::once;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move Properties")
		TEnumAsByte<ELookType::LookType> orientation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move Properties")
		UObject* orientationTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move Properties")
		float orientationSpeed = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool tickWhenPaused = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool ignoreTimeDilation = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		TEnumAsByte<EEventType::EventType> eventType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		FHitResult sweepResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool shouldTick = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool isTweenPaused = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		bool cullNonRenderedTweens = false;
	//Banked AActor->GetLastRenderTime()
	float lastSavedRenderTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		float secondsToWaitBeforeCull = 3.f;
	//Seconds since the last time AActor->GetLastRenderTime() was different from lastSavedRenderTime
	float timeSinceLastRendered = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		AActor* actorTweening = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		UWidget* widgetTweening = nullptr;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
	STweenableWidget* slateTweening = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		USceneComponent* componentTweening = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		UObject* onTweenStartTarget = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		UObject* onTweenUpdateTarget = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		UObject* onTweenLoopTarget = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		UObject* onTweenCompleteTarget = nullptr;

	STweenableWidget* onTweenStartSlateTarget = nullptr;
	STweenableWidget* onTweenTickSlateTarget = nullptr;
	STweenableWidget* onTweenLoopSlateTarget = nullptr;
	STweenableWidget* onTweenCompleteSlateTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		FName tweenName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		float tickTypeValue = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		float alpha = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		bool playingBackward = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		int32 numberOfLoopSections = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		int32 maximumLoopSections = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		bool actorTickableWhenPaused = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		float actorTimeDilation = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generic Properties")
		int32 tweenIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generic Properties")
		float delay = 0.f;

	//Spline Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Properties")
		USplineComponent* splineComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Properties")
		bool interpolateToSpline = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Properties")
		bool switchPathOrientationDirection = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Properties")
		float generatedPointDistance = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Properties")
		float PathLookDistance = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Properties")
		bool destroySplineObject = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Type Tween Values")
	struct FDataTypeValues dtv;

	//Vector Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector Properties")
		FVector vectorFrom = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector Properties")
		FVector vectorTo = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector Properties")
		FVector vectorTemp = FVector::ZeroVector;

	//Rotator Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotator Properties")
		FRotator rotatorFrom = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotator Properties")
		FRotator rotatorTo = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotator Properties")
		FRotator rotatorTemp = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotator Properties")
		bool shortestPath = false;

	//Vector2D Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector2D Properties")
		FVector2D vector2DFrom = FVector2D::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector2D Properties")
		FVector2D vector2DTo = FVector2D::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector2D Properties")
		FVector2D vector2DTemp = FVector2D::ZeroVector;

	//Float Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Float Properties")
		float floatFrom = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Float Properties")
		float floatTo = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Float Properties")
		float floatTemp = 0.f;

	//Linear Color Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Linear Color Properties")
		FLinearColor linearColorFrom = FLinearColor::Black;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Linear Color Properties")
		FLinearColor linearColorTo = FLinearColor::Black;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Linear Color Properties")
		FLinearColor linearColorTemp = FLinearColor::Black;

	//Delegate function names
	FString OnTweenStartFunctionName;
	FString OnTweenUpdateFunctionName;
	FString OnTweenLoopFunctionName;
	FString OnTweenCompleteFunctionName;

	//Methods
	UFUNCTION(BlueprintCallable, Category = "iTween")
		void SetTimerInterval(float interval = 0.f);

	UFUNCTION()
		void UpdateTween();

	UFUNCTION()
		void ParseParameters(FString params);

	UFUNCTION()
		void IgnorePauseTimeDilationSettings();

	UFUNCTION()
		void RestorePauseTimeDilationSettings();

	UFUNCTION()
		void SetTickTypeValue();

	UFUNCTION()
		float GetAlphaFromEquation(float value);

	UFUNCTION()
		float EaseInBounce(float value);

	UFUNCTION()
		float EaseOutBounce(float value);

	UFUNCTION()
		float EaseInOutBounce(float value);

	UFUNCTION()
		void EndPhase();

	UFUNCTION()
		void DelayTween();

	UFUNCTION()
		void LoopSectionStuff();

	UFUNCTION()
		void LoopWithDelay();

	UFUNCTION()
		void LoopWithoutDelay();

	UFUNCTION()
		void SpacializeValues();

	UFUNCTION()
		void SwitchValues();

	UFUNCTION()
		FString GetEventTypeEnumAsFString(EEventType::EventType enumValue);

	UFUNCTION()
		bool IsEventDataType();

	UFUNCTION()
		void RunInterface(UObject* target, ETweenInterfaceType::TweenInterfaceType type);

	//UFUNCTION()
	void RunSlateInterface(STweenableWidget* target, ETweenInterfaceType::TweenInterfaceType type);

	UFUNCTION()
		void CheckLoopType();

	UFUNCTION()
		void CheckIfFirstDelay();

	UFUNCTION()
		void BindDelegates();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Initialize Event"), Category = iTween)
		void InitEvent();

	//UFUNCTION()
	//	void NameEventActor();

	UFUNCTION()
		void CheckTargetValidity();

	UFUNCTION()
		void OrientToTargetFunction();

	UFUNCTION()
		void OrientToPathFunction();

	UFUNCTION()
		void ReconstructSpline();
};

