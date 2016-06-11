// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGGlobalGameInstance.h"

USGGlobalGameInstance::USGGlobalGameInstance()
{
	MessageEndpoint = FMessageEndpoint::Builder("GlobalGameInstance");
}