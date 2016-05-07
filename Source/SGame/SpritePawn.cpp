// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SpritePawn.h"


// Sets default values
ASpritePawn::ASpritePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RenderComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerPawnSprite"));
	RenderComponent->Mobility = EComponentMobility::Movable;
	RenderComponent->AttachParent = RootComponent;
}

// Called when the game starts or when spawned
void ASpritePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpritePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASpritePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

#if WITH_EDITOR
bool ASpritePawn::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);

	if (UPaperSprite* SourceSprite = RenderComponent->GetSprite())
	{
		Objects.Add(SourceSprite);
	}
	return true;
}
#endif

