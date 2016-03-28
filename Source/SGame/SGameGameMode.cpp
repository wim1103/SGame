// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGameGameMode.h"
#include "SGPlayerController.h"

ASGameGameMode::ASGameGameMode(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ASGPlayerController::StaticClass();
}

void ASGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}


