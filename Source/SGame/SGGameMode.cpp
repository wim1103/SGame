// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGGameMode.h"
#include "SGPlayerController.h"

ASGGameMode::ASGGameMode(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ASGPlayerController::StaticClass();
}

void ASGGameMode::BeginPlay()
{
	Super::BeginPlay();
}
