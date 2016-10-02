// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "iTweenPCH.h"
#include "iTAux.h"
#include "iTSpline.h"
#include "iTween.generated.h"

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

class AiTweenEvent;
class STweenableWidget;
UCLASS()
class SGAME_API UiTween : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//Methods

	UFUNCTION()
		static UWorld* GetWorldLocal();

public:

	//Methods
	//General Use
	UFUNCTION()
		static AiTweenEvent* SpawnEvent(AiTAux* aux);

	UFUNCTION()
		static AiTAux* GetAux();

	UFUNCTION()
		static void Print(FString message, FString type = "debug", float time = 5.f, bool printToLog = true);

	UFUNCTION()
		static UObject* FindObjectByName(FString s);

	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Actor Rotation In Parent Space"), Category = "iTween|Utilities")
	//static FRotator GetActorRotationByParent(AActor* actor);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Generate Spline From Vector Array"), Category = "iTween|Utilities")
		static void GenerateSplineFromVectorArray(AiTSpline* &owningActor, USplineComponent* &splineComponent, FVector referenceVector, FRotator referenceRotator, TArray<FVector> vectorArray, bool localToReference = false, bool closeSpline = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Generate Spline From Rotator Array"), Category = "iTween|Utilities")
		static void GenerateSplineFromRotatorArray(AiTSpline* &owningActor, USplineComponent* &splineComponent, FVector referenceVector, FRotator referenceRotator, TArray<FRotator> rotatorArray, float generatedPointDistance = 100.f, bool localToReference = false, bool closeSpline = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Utilities")
		static float GetDistanceBetweenTwoVectors(FVector sourceVector, FVector destination);

	//Constraints
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Utilities")
		static FVector ConstrainVector(FVector inputVector, FVector currentVector, EVectorConstraints::VectorConstraints vectorConstraints);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Utilities")
		static FVector2D ConstrainVector2D(FVector2D inputVector2D, FVector2D currentVector2D, EVector2DConstraints::Vector2DConstraints vector2DConstraints);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Utilities")
		static FRotator ConstrainRotator(FRotator inputRotator, FRotator currentRotator, ERotatorConstraints::RotatorConstraints rotatorConstraints);

	//Stopping, Pausing and Resuming Tweens
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void StopTweeningByIndex(int32 index);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void StopTweeningByTweeningObjectName(FName objectName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void StopTweeningByTweenName(FName tweenName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void StopTweeningByTweeningObjectReference(UObject* object);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void StopTweeningByEventReference(AiTweenEvent* object);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void StopAllTweens();

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void PauseTweeningByIndex(int32 index);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void PauseTweeningByTweeningObjectName(FName objectName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void PauseTweeningByTweenName(FName tweenName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void PauseTweeningByTweeningObjectReference(UObject* object);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void PauseTweeningByEventReference(AiTweenEvent* object);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void PauseAllTweens();

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void ResumeTweeningByIndex(int32 index);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void ResumeTweeningByTweeningObjectName(FName objectName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void ResumeTweeningByTweenName(FName tweenName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void ResumeTweeningByTweeningObjectReference(UObject* object);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void ResumeTweeningByEventReference(AiTweenEvent* object);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Stopping, Pausing and Resuming Tweens")
		static void ResumeAllTweens();

	//Get iTweenEvents
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Getting Event Operators")
		static AiTweenEvent* GetEventByIndex(int32 index);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Getting Event Operators")
		static TArray<AiTweenEvent*> GetEventsByTweeningObjectName(FName objectName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Getting Event Operators")
		static TArray<AiTweenEvent*> GetEventsByTweenName(FName tweenName = "No Name");

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Getting Event Operators")
		static TArray<AiTweenEvent*> GetEventsByTweeningObjectReference(UObject* object);

	//Actor
	//Actor Move From/To

	/**
	* Moves an actor from one location to another over time. Full customization options.
	* @return The generated actor that performs the tweening operation. Known as the "Event Operator."
	* @param Delegate An event delegate.
	* @param timerInterval Number of seconds to wait between tween updates. 0 will update the tween on every tick. For ~60 frames per second, use 0.016 seconds. For ~30 frames per second, use 0.033 seconds.
	* @param tweenName An optional name to give the tween. This is useful for stopping, pausing, or resuming tweens by name among other things.
	* @param actorToMove The actor you'd like to move.
	* @param locationFrom The vector from which the actor will start the tween.
	* @param locationTo The vector at which the actor will end the tween.
	* @param enforceValueTo Whether or not to force the tweening object to the 'to' value. Normally you'll want to leave this on to ensure that the value you tween to is exact, but if you're using a custom EaseType curve that ends on any value other than 1.0, turn this option off.
	* @param vectorConstraints You can constrain your Vector-based tween to only aimate along specific axes. xOnly, yOnly, zOnly, xyOnly, xzOnly, or yzOnly.
	* @param coordinateSpace The space to which the vectors will be relative. World, parent, or self.
	* @param sweep Whether or not to stop the tween if the object collides with another.
	* @param delay The number of seconds to wait before starting a tween or before a loop section, depending on your delayType.
	* @param delayType When to enforce a delay. Before the tween begins, after a loop section, after a full loop, or a combination.
	* @param tickType 'Seconds': The tween should take a specified number of seconds to complete. 'Speed': The tween should should be performed at a specified speed. The 'Speed' value is interpreted differently for different types of tweening operations.
	* @param tickTypeValue The value to specify. Relates to tickType. For example, if your tickType is seconds and your tickTypeValue is 3, the tween will take 3 seconds to complete.
	* @param easeType How to ease the tween or modulate its speed.
	* @param customEaseTypeCurve If using the EaseType 'Custom Curve,' you can choose a CurveFloat you've created and saved to drive the easing of the tween.
	* @param punchAmplitude When using the 'punch' EaseType, this determines the 'power' or 'violence' level of the punch animation. If not using 'punch,' this value does nothing.
	* @param loopType Whether or not to loop the tween and how. 'Play Once' will not loop the tween. 'Rewind' will snap the tween back to its origin after it completes and start over again. 'Back-and-Forth (Ping-Pong)' will cause the tweening object to tween back to its original value after it reaches its destination.
	* @param maximumLoopSections If using any loopType that is not 'Play Once,' this sets the maximum number of times the tween will pass a loop section before automatically terminating. In 'Rewind' mode, a loop section is when the tween snaps back. In 'Back-and-Forth (Ping-Pong)' mode, a loop section is when a loop changes direction to go back to its original value. A tween that has gone to its destination and come back in 'Back-and-Forth (Ping-Pong)' mode has completed two loop sections.
	* @param orientation Whether or not to change the object's rotation and if so, whether to rotate toward the path of travel or another object (target).
	* @param orientationTarget The object toward which the tweening object will rotate to look if 'Orient to Target' is chosen.
	* @param orientationSpeed If using an orientation other than 'No Orientation Change,' this is an arbitrary speed value that will determine how quickly the object will rotate toward the specified direction.
	* @param rotatorConstraints If using an orientation other than 'No Orientation Change,' this option can constrain the rotation to only certain axes. pitchOnly, yawOnly, rollOnly, pitchYawOnly, pitchRollOnly, or yawRollOnly.
	* @param onTweenStartTarget The object to which the tween will send an OnTweenStart interface message.
	* @param onTweenUpdateTarget The object to which the tween will send an OnTweenUpdate interface message.
	* @param onTweenLoopTarget The object to which the tween will send an OnTweenLoop interface message.
	* @param onTweenCompleteTarget The object to which the tween will send an OnTweenComplete interface message.
	* @param tieToObjectValidity If enabled, the tween will terminate itself if the object upon which it acts is destroyed. If disabled, the tween will continue after the object is destroyed. You may want to leave this unchecked if you plan to manually switch targets during the same tween when one is destroyed.
	* @param cullNonRenderedTweens If enabled, iTween will check to see if the object is visible before executing and if not, will continue until secondsToWaitBeforeCull has elapsed, then stop executing until it's seen by the camera. The tween will keep crunching numbers, but the object won't transform.
	* @param secondsToWaitBeforeCull Number of seconds to wait when an object is offscreen before no longer updating its transform. Only applicable when cullNonRenderedTweens is true.
	*/
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move From/To (Full)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", AActor* actorToMove = nullptr, FVector locationFrom = FVector::ZeroVector, FVector locationTo = FVector::ZeroVector, bool enforceValueTo = true, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool sweep = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	/**
	* Moves an actor from one location to another over time. Most-used options only. Parameters available.
	* @return The generated actor that performs the tweening operation. Known as the "Event Operator."
	* @param tweenName An optional name to give the tween. This is useful for stopping, pausing, or resuming tweens by name among other things.
	* @param actorToMove The actor you'd like to move.
	* @param locationFrom The vector from which the actor will start the tween.
	* @param locationTo The vector at which the actor will end the tween.
	* @param coordinateSpace The space to which the vectors will be relative. World, parent, or self.
	* @param sweep Whether or not to stop the tween if the object collides with another.
	* @param timeInSeconds Number of seconds over which you'd like the tween to take place.
	* @param easeType How to ease the tween or modulate its speed.
	* @param parameters Special text instructions for the tween. See https://wiki.unrealengine.com/Itween for more information.
	* @param onTweenStartTarget The object to which the tween will send an OnTweenStart interface message.
	* @param onTweenUpdateTarget The object to which the tween will send an OnTweenUpdate interface message.
	* @param onTweenCompleteTarget The object to which the tween will send an OnTweenComplete interface message.
	*/
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move From/To (Simple)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveFromToSimple(FName tweenName = "No Name", AActor* actorToMove = nullptr, FVector locationFrom = FVector::ZeroVector, FVector locationTo = FVector::ZeroVector, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool sweep = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	/**
	* Moves an actor from one location to another over time. Parameters and object inputs only.
	* @return The generated actor that performs the tweening operation. Known as the "Event Operator."
	* @param actorToMove The actor you'd like to move.
	* @param parameters Special text instructions for the tween. See https://wiki.unrealengine.com/Itween for more information.
	* @param initializeOnSpawn If enabled, the tween will start immediately. If disabled, you will have to call "Initialize Event" from the Event Operator. This is useful if you want to set up a tween, but want it to start with a custom event or you need to set some more variables and want to be sure they're post-set before the tween actually begins.
	* @param customEaseTypeCurve If using the EaseType 'Custom Curve,' you can choose a CurveFloat you've created and saved to drive the easing of the tween.
	* @param orientationTarget The object toward which the tweening object will rotate to look.
	* @param onTweenStartTarget The object to which the tween will send an OnTweenStart interface message.
	* @param onTweenUpdateTarget The object to which the tween will send an OnTweenUpdate interface message.
	* @param onTweenLoopTarget The object to which the tween will send an OnTweenLoop interface message.
	* @param onTweenCompleteTarget The object to which the tween will send an OnTweenComplete interface message.
	*/
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move From/To (Expert)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveFromToExpert(AActor* actorToMove = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* orientationTarget = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	/**
	* Moves an actor from one location to another over time. Parameters and object inputs only.
	* @return The generated actor that performs the tweening operation. Known as the "Event Operator."
	* @param actorToMove The actor you'd like to move.
	* @param parameters Special text instructions for the tween. See https://wiki.unrealengine.com/Itween for more information.
	* @param initializeOnSpawn If enabled, the tween will start immediately. If disabled, you will have to call "Initialize Event" from the Event Operator. This is useful if you want to set up a tween, but want it to start with a custom event or you need to set some more variables and want to be sure they're post-set before the tween actually begins.
	*/
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move From/To (Minimal)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveFromToMin(AActor* actorToMove = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Actor Move To Spline Point
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move To Spline Point (Full)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveToSplinePointFull(float timerInterval = 0.f, FName tweenName = "No Name", AActor* actorToMove = nullptr, USplineComponent* splineComponent = nullptr, bool moveToPath = false, bool enforceValueTo = true, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, bool sweep = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, bool switchPathOrientationDirection = true, ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool destroySplineComponent = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move To Spline Point (Simple)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveToSplinePointSimple(FName tweenName = "No Name", AActor* actorToMove = nullptr, USplineComponent* splineComponent = nullptr, bool moveToPath = false, bool sweep = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr, bool destroySplineComponent = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move To Spline Point (Expert)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveToSplinePointExpert(AActor* actorToMove = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* orientationTarget = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Move To Spline Point (Minimal)"), Category = "iTween|Events|Actor Tweens|Actor Move")
		static AiTweenEvent* ActorMoveToSplinePointMin(AActor* actorToMove = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Actor Move Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Events|Actor Tweens|Actor Move")
		static void ActorMoveUpdate(AActor* actorToMove, FVector locationTo, float delta, float speed = 5.f, bool isLocal = false, bool sweep = false, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none);

	//Actor Rotate From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate From/To (Full)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", AActor* actorToRotate = nullptr, FRotator rotationFrom = FRotator::ZeroRotator, FRotator rotationTo = FRotator::ZeroRotator, bool enforceValueTo = true, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool shortestPath = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate From/To (Simple)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateFromToSimple(FName tweenName = "No Name", AActor* actorToRotate = nullptr, FRotator rotationFrom = FRotator::ZeroRotator, FRotator rotationTo = FRotator::ZeroRotator, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool shortestPath = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate From/To (Expert)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateFromToExpert(AActor* actorToRotate = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate From/To (Minimal)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateFromToMin(AActor* actorToRotate = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Actor Rotate To Spline Point
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate To Spline Point (Full)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateToSplinePointFull(float timerInterval = 0.f, FName tweenName = "No Name", AActor* actorToRotate = nullptr, USplineComponent* splineComponent = nullptr, bool rotateToPath = false, bool enforceValueTo = true, float generatedPointDistance = 100.f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool destroySplineComponent = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate To Spline Point (Simple)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateToSplinePointSimple(FName tweenName = "No Name", AActor* actorToRotate = nullptr, USplineComponent* splineComponent = nullptr, bool rotateToPath = false, float generatedPointDistance = 100.f, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr, bool destroySplineComponent = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate To Spline Point (Expert)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateToSplinePointExpert(AActor* actorToRotate = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* orientationTarget = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Rotate To Spline Point (Minimal)"), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static AiTweenEvent* ActorRotateToSplinePointMin(AActor* actorToRotate = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Actor Rotate Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Events|Actor Tweens|Actor Rotate")
		static void ActorRotateUpdate(AActor* actorToRotate, FRotator rotationTo, float delta, float speed = 5.f, bool isLocal = false, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none);

	//Actor Scale From To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Scale From/To (Full)"), Category = "iTween|Events|Actor Tweens|Actor Scale")
		static AiTweenEvent* ActorScaleFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", AActor* actorToScale = nullptr, FVector scaleFrom = FVector::ZeroVector, FVector scaleTo = FVector(1, 1, 1), bool enforceValueTo = true, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, bool isLocal = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Scale From/To (Simple)"), Category = "iTween|Events|Actor Tweens|Actor Scale")
		static AiTweenEvent* ActorScaleFromToSimple(FName tweenName = "No Name", AActor* actorToScale = nullptr, FVector scaleFrom = FVector::ZeroVector, FVector scaleTo = FVector(1, 1, 1), bool isLocal = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Scale From/To (Expert)"), Category = "iTween|Events|Actor Tweens|Actor Scale")
		static AiTweenEvent* ActorScaleFromToExpert(AActor* actorToScale = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Actor Scale From/To (Minimal)"), Category = "iTween|Events|Actor Tweens|Actor Scale")
		static AiTweenEvent* ActorScaleFromToMin(AActor* actorToScale = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Actor Scale Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Events|Actor Tweens|Actor Scale")
		static void ActorScaleUpdate(AActor* actorToScale, FVector scaleTo, float delta, float speed = 5.f, bool isLocal = false, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none);
	//Component
	//Component Move From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move From/To (Full)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", USceneComponent* componentToMove = nullptr, FVector locationFrom = FVector::ZeroVector, FVector locationTo = FVector::ZeroVector, bool enforceValueTo = true, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool sweep = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move From/To (Simple)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveFromToSimple(FName tweenName = "No Name", USceneComponent* componentToMove = nullptr, FVector locationFrom = FVector::ZeroVector, FVector locationTo = FVector::ZeroVector, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool sweep = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move From/To (Expert)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveFromToExpert(USceneComponent* componentToMove = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* orientationTarget = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move From/To (Minimal)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveFromToMin(USceneComponent* componentToMove = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Component Move To Spline Point
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move To Spline Point (Full)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveToSplinePointFull(float timerInterval = 0.f, FName tweenName = "No Name", USceneComponent* componentToMove = nullptr, USplineComponent* splineComponent = nullptr, bool moveToPath = false, bool enforceValueTo = true, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, bool sweep = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, bool switchPathOrientationDirection = true, ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool destroySplineComponent = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move To Spline Point (Simple)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveToSplinePointSimple(FName tweenName = "No Name", USceneComponent* componentToMove = nullptr, USplineComponent* splineComponent = nullptr, bool moveToPath = false, bool sweep = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr, bool destroySplineComponent = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move To Spline Point (Expert)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveToSplinePointExpert(USceneComponent* componentToMove = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* orientationTarget = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Move To Spline Point (Minimal)"), Category = "iTween|Events|Component Tweens|Component Move")
		static AiTweenEvent* ComponentMoveToSplinePointMin(USceneComponent* componentToMove = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Component Move Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Events|Component Tweens|Component Move")
		static void ComponentMoveUpdate(USceneComponent* componentToMove, FVector locationTo, float delta, float speed = 5.f, bool isLocal = false, bool sweep = false, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none);

	//Component Rotate From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate From/To (Full)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", USceneComponent* componentToRotate = nullptr, FRotator rotationFrom = FRotator::ZeroRotator, FRotator rotationTo = FRotator::ZeroRotator, bool enforceValueTo = true, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool shortestPath = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate From/To (Simple)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateFromToSimple(FName tweenName = "No Name", USceneComponent* componentToRotate = nullptr, FRotator rotationFrom = FRotator::ZeroRotator, FRotator rotationTo = FRotator::ZeroRotator, ECoordinateSpace::CoordinateSpace coordinateSpace = CoordinateSpace::world, bool shortestPath = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate From/To (Expert)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateFromToExpert(USceneComponent* componentToRotate = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate From/To (Minimal)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateFromToMin(USceneComponent* componentToRotate = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Component Rotate To Spline Point
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate To Spline Point (Full)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateToSplinePointFull(float timerInterval = 0.f, FName tweenName = "No Name", USceneComponent* componentToRotate = nullptr, USplineComponent* splineComponent = nullptr, bool rotateToPath = false, bool enforceValueTo = true, float generatedPointDistance = 100.f, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool destroySplineComponent = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate To Spline Point (Simple)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateToSplinePointSimple(FName tweenName = "No Name", USceneComponent* componentToRotate = nullptr, USplineComponent* splineComponent = nullptr, bool rotateToPath = false, float generatedPointDistance = 100.f, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr, bool destroySplineComponent = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate To Spline Point (Expert)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateToSplinePointExpert(USceneComponent* componentToRotate = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* orientationTarget = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Rotate To Spline Point (Minimal)"), Category = "iTween|Events|Component Tweens|Component Rotate")
		static AiTweenEvent* ComponentRotateToSplinePointMin(USceneComponent* componentToRotate = nullptr, USplineComponent* splineComponent = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Component Rotate Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Events|Component Tweens|Component Rotate")
		static void ComponentRotateUpdate(USceneComponent* componentToRotate, FRotator rotationTo, float delta, float speed = 5.f, bool isLocal = false, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none);

	//Component Scale From To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Scale From/To (Full)"), Category = "iTween|Events|Component Tweens|Component Scale")
		static AiTweenEvent* ComponentScaleFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", USceneComponent* componentToScale = nullptr, FVector scaleFrom = FVector::ZeroVector, FVector scaleTo = FVector(1, 1, 1), bool enforceValueTo = true, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, bool isLocal = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true, bool cullNonRenderedTweens = false, float secondsToWaitBeforeCull = 3.f);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Scale From/To (Simple)"), Category = "iTween|Events|Component Tweens|Component Scale")
		static AiTweenEvent* ComponentScaleFromToSimple(FName tweenName = "No Name", USceneComponent* componentToScale = nullptr, FVector scaleFrom = FVector::ZeroVector, FVector scaleTo = FVector(1, 1, 1), bool isLocal = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Scale From/To (Expert)"), Category = "iTween|Events|Component Tweens|Component Scale")
		static AiTweenEvent* ComponentScaleFromToExpert(USceneComponent* componentToScale = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Component Scale From/To (Minimal)"), Category = "iTween|Events|Component Tweens|Component Scale")
		static AiTweenEvent* ComponentScaleFromToMin(USceneComponent* componentToScale = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Component Scale Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction), Category = "iTween|Events|Component Tweens|Component Scale")
		static void ComponentScaleUpdate(USceneComponent* componentToScale, FVector scaleTo, float delta, float speed = 5.f, bool isLocal = false, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none);

	//UMG Render Transforms
	//Render Translation Move From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Move From/To (Full)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Move (Translate)")
		static AiTweenEvent* UMGRTMoveFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", UWidget* widgetToMove = nullptr, FVector2D locationFrom = FVector2D::ZeroVector, FVector2D locationTo = FVector2D::ZeroVector, bool enforceValueTo = true, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, /*ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, */UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Move From/To (Simple)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Move (Translate)")
		static AiTweenEvent* UMGRTMoveFromToSimple(FName tweenName = "No Name", UWidget* widgetToMove = nullptr, FVector2D locationFrom = FVector2D::ZeroVector, FVector2D locationTo = FVector2D::ZeroVector, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Move From/To (Expert)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Move (Translate)")
		static AiTweenEvent* UMGRTMoveFromToExpert(UWidget* widgetToMove = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, /*UObject* orientationTarget = nullptr, */UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Move From/To (Minimal)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Move (Translate)")
		static AiTweenEvent* UMGRTMoveFromToMin(UWidget* widgetToMove = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Render Translation Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Move Update"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Move (Translate)")
		static void UMGRTMoveUpdate(UWidget* widgetToMove, FVector2D locationTo, float delta, float speed = 5.f, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none/*, ELookType::LookType orientation = noOrientationChange, UObject* orientationTarget = nullptr, float orientationSpeed = 5.0f, */);

	//Render Angle From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Rotate From/To (Full)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Rotate (Angle)")
		static AiTweenEvent* UMGRTRotateFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", UWidget* widgetToRotate = nullptr, float angleFrom = 0, float angleTo = 90, bool enforceValueTo = true, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Rotate From/To (Simple)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Rotate (Angle)")
		static AiTweenEvent* UMGRTRotateFromToSimple(FName tweenName = "No Name", UWidget* widgetToRotate = nullptr, float angleFrom = 0, float angleTo = 90, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Rotate From/To (Expert)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Rotate (Angle)")
		static AiTweenEvent* UMGRTRotateFromToExpert(UWidget* widgetToRotate = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Rotate From/To (Minimal)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Rotate (Angle)")
		static AiTweenEvent* UMGRTRotateFromToMin(UWidget* widgetToRotate = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Render Rotate Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Rotate Update"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Rotate (Angle)")
		static void UMGRTRotateUpdate(UWidget* widgetToRotate, float angleTo, float delta, float speed = 5.f);

	//Render Scale From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Scale From/To (Full)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Scale")
		static AiTweenEvent* UMGRTScaleFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", UWidget* widgetToScale = nullptr, FVector2D scaleFrom = FVector2D(1, 1), FVector2D scaleTo = FVector2D(2, 2), bool enforceValueTo = true, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Scale From/To (Simple)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Scale")
		static AiTweenEvent* UMGRTScaleFromToSimple(FName tweenName = "No Name", UWidget* widgetToScale = nullptr, FVector2D scaleFrom = FVector2D(1, 1), FVector2D scaleTo = FVector2D(2, 2), float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Scale From/To (Expert)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Scale")
		static AiTweenEvent* UMGRTScaleFromToExpert(UWidget* widgetToScale = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Scale From/To (Minimal)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Scale")
		static AiTweenEvent* UMGRTScaleFromToMin(UWidget* widgetToScale = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Render Scale Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Scale Update"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Scale")
		static void UMGRTScaleUpdate(UWidget* widgetToScale, FVector2D scaleTo, float delta, float speed = 5.f, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none);

	//Render Shear From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Shear From/To (Full)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Shear")
		static AiTweenEvent* UMGRTShearFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", UWidget* widgetToShear = nullptr, FVector2D shearFrom = FVector2D::ZeroVector, FVector2D shearTo = FVector2D(5, 5), bool enforceValueTo = true, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false, bool tieToObjectValidity = true);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Shear From/To (Simple)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Shear")
		static AiTweenEvent* UMGRTShearFromToSimple(FName tweenName = "No Name", UWidget* widgetToShear = nullptr, FVector2D shearFrom = FVector2D::ZeroVector, FVector2D shearTo = FVector2D(5, 5), float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Shear From/To (Expert)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Shear")
		static AiTweenEvent* UMGRTShearFromToExpert(UWidget* widgetToShear = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Shear From/To (Minimal)"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Shear")
		static AiTweenEvent* UMGRTShearFromToMin(UWidget* widgetToShear = nullptr, FString parameters = "", bool initializeOnSpawn = true);

	//Render Shear Update
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "UMG RT Shear Update"), Category = "iTween|Events|UMG Render Transform Tweens|UMG RT Shear")
		static void UMGRTShearUpdate(UWidget* widgetToShear, FVector2D shearTo, float delta, float speed = 5.f, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none);

	//Data Types
	//Float From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Float From/To (Full)"), Category = "iTween|Events|Data Tweens|Float From/To")
		static AiTweenEvent* FloatFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", float startValue = 0.f, float endValue = 1.f, bool enforceValueTo = true, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Float From/To (Simple)"), Category = "iTween|Events|Data Tweens|Float From/To")
		static AiTweenEvent* FloatFromToSimple(FName tweenName = "No Name", float startValue = 0.f, float endValue = 1.f, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Float From/To (Expert)"), Category = "iTween|Events|Data Tweens|Float From/To")
		static AiTweenEvent* FloatFromToExpert(FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Float From/To (Minimal)"), Category = "iTween|Events|Data Tweens|Float From/To")
		static AiTweenEvent* FloatFromToMin(FString parameters = "", bool initializeOnSpawn = true);

	//Vector From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector From/To (Full)"), Category = "iTween|Events|Data Tweens|Vector From/To")
		static AiTweenEvent* VectorFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", FVector startValue = FVector::ZeroVector, FVector endValue = FVector::ZeroVector, bool enforceValueTo = true, EVectorConstraints::VectorConstraints vectorConstraints = VectorConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector From/To (Simple)"), Category = "iTween|Events|Data Tweens|Vector From/To")
		static AiTweenEvent* VectorFromToSimple(FName tweenName = "No Name", FVector startValue = FVector::ZeroVector, FVector endValue = FVector::ZeroVector, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector From/To (Expert)"), Category = "iTween|Events|Data Tweens|Vector From/To")
		static AiTweenEvent* VectorFromToExpert(FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector From/To (Minimal)"), Category = "iTween|Events|Data Tweens|Vector From/To")
		static AiTweenEvent* VectorFromToMin(FString parameters = "", bool initializeOnSpawn = true);

	//Vector2D From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector2D From/To (Full)"), Category = "iTween|Events|Data Tweens|Vector2D From/To")
		static AiTweenEvent* Vector2DFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", FVector2D startValue = FVector2D::ZeroVector, FVector2D endValue = FVector2D::ZeroVector, bool enforceValueTo = true, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector2D From/To (Simple)"), Category = "iTween|Events|Data Tweens|Vector2D From/To")
		static AiTweenEvent* Vector2DFromToSimple(FName tweenName = "No Name", FVector2D startValue = FVector2D::ZeroVector, FVector2D endValue = FVector2D::ZeroVector, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector2D From/To (Expert)"), Category = "iTween|Events|Data Tweens|Vector2D From/To")
		static AiTweenEvent* Vector2DFromToExpert(FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Vector2D From/To (Minimal)"), Category = "iTween|Events|Data Tweens|Vector2D From/To")
		static AiTweenEvent* Vector2DFromToMin(FString parameters = "", bool initializeOnSpawn = true);

	//Rotator From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Rotator From/To (Full)"), Category = "iTween|Events|Data Tweens|Rotator From/To")
		static AiTweenEvent* RotatorFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", FRotator startValue = FRotator::ZeroRotator, FRotator endValue = FRotator::ZeroRotator, bool enforceValueTo = true, ERotatorConstraints::RotatorConstraints rotatorConstraints = RotatorConstraints::none, bool shortestPath = false, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Rotator From/To (Simple)"), Category = "iTween|Events|Data Tweens|Rotator From/To")
		static AiTweenEvent* RotatorFromToSimple(FName tweenName = "No Name", FRotator startValue = FRotator::ZeroRotator, FRotator endValue = FRotator::ZeroRotator, bool shortestPath = false, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Rotator From/To (Expert)"), Category = "iTween|Events|Data Tweens|Rotator From/To")
		static AiTweenEvent* RotatorFromToExpert(FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Rotator From/To (Minimal)"), Category = "iTween|Events|Data Tweens|Rotator From/To")
		static AiTweenEvent* RotatorFromToMin(FString parameters = "", bool initializeOnSpawn = true);

	//Linear Color From/To
	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Linear Color From/To (Full)"), Category = "iTween|Events|Data Tweens|Linear Color From/To")
		static AiTweenEvent* LinearColorFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", FLinearColor startValue = FLinearColor::White, FLinearColor endValue = FLinearColor::Black, bool enforceValueTo = true, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, int32 maximumLoopSections = 0, UObject* onTweenStartTarget = nullptr, FString OnTweenStartFunctionName = "", UObject* onTweenUpdateTarget = nullptr, FString OnTweenUpdateFunctionName = "", UObject* onTweenLoopTarget = nullptr, FString OnTweenLoopFunctionName = "", UObject* onTweenCompleteTarget = nullptr, FString OnTweenCompleteFunctionName = "", bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Linear Color From/To (Simple)"), Category = "iTween|Events|Data Tweens|Linear Color From/To")
		static AiTweenEvent* LinearColorFromToSimple(FName tweenName = "No Name", FLinearColor startValue = FLinearColor::White, FLinearColor endValue = FLinearColor::Black, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Linear Color From/To (Expert)"), Category = "iTween|Events|Data Tweens|Linear Color From/To")
		static AiTweenEvent* LinearColorFromToExpert(FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, UObject* onTweenStartTarget = nullptr, UObject* onTweenUpdateTarget = nullptr, UObject* onTweenLoopTarget = nullptr, UObject* onTweenCompleteTarget = nullptr);

	UFUNCTION(BlueprintCallable, meta = (UnsafeDuringActorConstruction, DisplayName = "Linear Color From/To (Minimal)"), Category = "iTween|Events|Data Tweens|Linear Color From/To")
		static AiTweenEvent* LinearColorFromToMin(FString parameters = "", bool initializeOnSpawn = true);

	//Slate Data Types
	//Float From/To
	static AiTweenEvent* SlateFloatFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", float startValue = 0.f, float endValue = 1.f, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, STweenableWidget* onTweenStartTarget = nullptr, STweenableWidget* onTweenTickTarget = nullptr, STweenableWidget* onTweenLoopTarget = nullptr, STweenableWidget* onTweenCompleteTarget = nullptr, bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	static AiTweenEvent* SlateFloatFromToSimple(FName tweenName = "No Name", float startValue = 0.f, float endValue = 1.f, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", STweenableWidget* onTweenStartTarget = nullptr, STweenableWidget* onTweenTickTarget = nullptr, STweenableWidget* onTweenCompleteTarget = nullptr);

	static AiTweenEvent* SlateFloatFromToExpert(FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, STweenableWidget* onTweenStartTarget = nullptr, STweenableWidget* onTweenTickTarget = nullptr, STweenableWidget* onTweenLoopTarget = nullptr, STweenableWidget* onTweenCompleteTarget = nullptr);

	//Slate Render Transforms
	//Render Translation Move From/To
	static AiTweenEvent* SlateMoveFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", STweenableWidget*  widgetToMove = nullptr, FVector2D locationFrom = FVector2D::ZeroVector, FVector2D locationTo = FVector2D::ZeroVector, EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, /*ELookType::LookType orientation = noOrientationChange, STweenableWidget*  orientationTarget = nullptr, float orientationSpeed = 5.0f, */STweenableWidget*  onTweenStartTarget = nullptr, STweenableWidget*  onTweenTickTarget = nullptr, STweenableWidget*  onTweenLoopTarget = nullptr, STweenableWidget*  onTweenCompleteTarget = nullptr, bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	static AiTweenEvent* SlateMoveFromToSimple(FName tweenName = "No Name", STweenableWidget*  widgetToMove = nullptr, FVector2D locationFrom = FVector2D::ZeroVector, FVector2D locationTo = FVector2D::ZeroVector, float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", STweenableWidget*  onTweenStartTarget = nullptr, STweenableWidget*  onTweenTickTarget = nullptr, STweenableWidget*  onTweenCompleteTarget = nullptr);

	static AiTweenEvent* SlateMoveFromToExpert(STweenableWidget*  widgetToMove = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, /*STweenableWidget*  orientationTarget = nullptr, */STweenableWidget*  onTweenStartTarget = nullptr, STweenableWidget*  onTweenTickTarget = nullptr, STweenableWidget*  onTweenLoopTarget = nullptr, STweenableWidget*  onTweenCompleteTarget = nullptr);

	//Render Scale From/To
	static AiTweenEvent* SlateScaleFromToFull(float timerInterval = 0.f, FName tweenName = "No Name", STweenableWidget*  widgetToScale = nullptr, FVector2D scaleFrom = FVector2D(1, 1), FVector2D scaleTo = FVector2D(2, 2), EVector2DConstraints::Vector2DConstraints vector2DConstraints = Vector2DConstraints::none, float delay = 0.0f, EDelayType::DelayType delayType = first, ETickType::TickType tickType = seconds, float tickTypeValue = 5.0f, EEaseType::EaseType easeType = linear, UCurveFloat* customEaseTypeCurve = nullptr, float punchAmplitude = 1.0f, ELoopType::LoopType loopType = once, STweenableWidget*  onTweenStartTarget = nullptr, STweenableWidget*  onTweenTickTarget = nullptr, STweenableWidget*  onTweenLoopTarget = nullptr, STweenableWidget*  onTweenCompleteTarget = nullptr, bool tickWhenPaused = false, bool ignoreTimeDilation = false);

	static AiTweenEvent* SlateScaleFromToSimple(FName tweenName = "No Name", STweenableWidget* widgetToScale = nullptr, FVector2D scaleFrom = FVector2D(1, 1), FVector2D scaleTo = FVector2D(2, 2), float timeInSeconds = 5.0f, EEaseType::EaseType easeType = linear, FString parameters = "", STweenableWidget*  onTweenStartTarget = nullptr, STweenableWidget*  onTweenTickTarget = nullptr, STweenableWidget*  onTweenCompleteTarget = nullptr);

	static AiTweenEvent* SlateScaleFromToExpert(STweenableWidget*  widgetToScale = nullptr, FString parameters = "", bool initializeOnSpawn = true, UCurveFloat* customEaseTypeCurve = nullptr, STweenableWidget*  onTweenStartTarget = nullptr, STweenableWidget*  onTweenTickTarget = nullptr, STweenableWidget*  onTweenLoopTarget = nullptr, STweenableWidget*  onTweenCompleteTarget = nullptr);

};
