// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "iTweenPCH.h"
#include "iTween.h"
#include "iTInterface.h"

UiTInterface::UiTInterface(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

#if !PLATFORM_ANDROID
void STweenableWidget::OnTweenStart(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName)
{
	UiTween::Print("Interface message sending...");
}

void STweenableWidget::OnTweenTick(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, float alphaCompletion)
{
	UiTween::Print("Interface message sending...");
}

void STweenableWidget::OnTweenDataTick(AiTweenEvent* eventOperator, FName tweenName, float floatValue, FLinearColor linearColorValue, FRotator rotatorValue, FVector vectorValue, FVector2D vector2DValue, float alphaCompletion)
{
	UiTween::Print("Interface message sending...");
}

void STweenableWidget::OnTweenLoop(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, int32 numberOfLoopSections, ELoopType::LoopType loopType, bool playingBackward)
{
	UiTween::Print("Interface message sending...");
}

void STweenableWidget::OnTweenComplete(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform)
{
	UiTween::Print("Interface message sending...");
}

void STweenableWidget::SetTweenScale(FVector2D Scale)
{
	TweenTransform.Scale = Scale;
	UpdateRenderTransform();
}

void STweenableWidget::SetTweenShear(FVector2D Shear)
{
	TweenTransform.Shear = Shear;
	UpdateRenderTransform();
}

void STweenableWidget::SetTweenAngle(float Angle)
{
	TweenTransform.Angle = Angle;
	UpdateRenderTransform();
}

void STweenableWidget::SetTweenTranslation(FVector2D Translation)
{
	TweenTransform.Translation = Translation;
	UpdateRenderTransform();
}

void STweenableWidget::UpdateRenderTransform()
{
	if (!TweenTransform.IsIdentity())
	{
		//we need to create the transform here, you can not extract the original values so it must be recreated every time there is a change
		FSlateRenderTransform FinalTransform = ::Concatenate(FScale2D(TweenTransform.Scale), FShear2D::FromShearAngles(TweenTransform.Shear), FQuat2D(FMath::DegreesToRadians(TweenTransform.Angle)), FVector2D(TweenTransform.Translation));
		this->SetRenderTransform(FinalTransform);
	}
	else
	{
		this->SetRenderTransform(TOptional<FSlateRenderTransform>());
	}
}
#endif

void IiTInterface::OnTweenStartNative(AiTweenEvent* eventOperator, AActor* actorTweening /*= nullptr*/, USceneComponent* componentTweening /*= nullptr*/, UWidget* widgetTweening /*= nullptr*/, FName tweenName /*= ""*/)
{
	UiTween::Print("Interface message sending...");
}

void IiTInterface::OnTweenUpdateNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FDataTypeValues dataTypeValues, float alphaCompletion /*= 0.f*/)
{
	UiTween::Print("Interface message sending...");
}

void IiTInterface::OnTweenLoopNative(AiTweenEvent* eventOperator, AActor* actorTweening /*= nullptr*/, USceneComponent* componentTweening /*= nullptr*/, UWidget* widgetTweening /*= nullptr*/, FName tweenName /*= ""*/, int32 numberOfLoopSections /*= 0*/, ELoopType::LoopType loopType /*= once*/, bool playingBackward /*= false*/)
{
	UiTween::Print("Interface message sending...");
}

void IiTInterface::OnTweenCompleteNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform)
{
	UiTween::Print("Interface message sending...");
}
