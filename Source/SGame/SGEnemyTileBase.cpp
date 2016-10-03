// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGGameMode.h"
#include "SGEnemyTileBase.h"

ASGEnemyTileBase::ASGEnemyTileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackingElapsedTime = 0;
	AttackingDuration = 1.0f;
	AttackingShakeDegree = 10.0f;
	AttackingShakeFreq = 6;
	AttackingScaleTimeWindow = 0.1f;
	AttackingScaleRatio = 1.3f;

	Text_Attack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent-Attack"));
	Text_Attack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Text_Armor = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent-Armor"));
	Text_Armor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Text_HP = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent-HP"));
	Text_HP->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASGEnemyTileBase::EnemyAttack()
{
	// Enemy only attack next round
	ASGGameMode* GameMode = Cast<ASGGameMode>(UGameplayStatics::GetGameMode(this));
	checkSlow(GameMode);
	int CurrentRound = GameMode->GetCurrentRound();
	if (CurrentRound != 1 && CurrentRound - SpawnedRound == 0)
	{
		// Skip the spawn round except for the first round
		return;
	}
	
	StartAttackAnimation();
}

void ASGEnemyTileBase::ResetTile()
{
	// Set the flag, disable the attacking tick
	bIsAttacking = false;

	// Pop down the tile to the original place
	this->AddActorWorldOffset(FVector(0.0f, -1000.0f, 0.0f));
	this->SetActorLocation(FallingEndLocation);

	// Make sure the rotation and scale back to origin
	checkSlow(Sprite_Normal);
	checkSlow(GetRenderComponent());
	GetRenderComponent()->SetSprite(Sprite_Normal);
	GetRenderComponent()->SetRelativeRotation(FRotator(0, 0, 0));
	GetRenderComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void ASGEnemyTileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASGEnemyTileBase::BeginPlay()
{
	Super::BeginPlay();

	// Create its own message endpoint, noted that this class may
	// have two message endpoint, one for its parent messages and handlers, and 
	// one for itself
	FString EndPointName = FString::Printf(TEXT("Gameplay_Tile_%d_Enemylogic"), GridAddress);
	MessageEndpoint = FMessageEndpoint::Builder(*EndPointName)
		.Handling<FMessage_Gameplay_EnemyBeginAttack>(this, &ASGEnemyTileBase::HandleBeginAttack)
		.Handling<FMessage_Gameplay_EnemyGetHit>(this, &ASGEnemyTileBase::HandlePlayHit);

	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the tile need events
		MessageEndpoint->Subscribe<FMessage_Gameplay_EnemyBeginAttack>();
		MessageEndpoint->Subscribe<FMessage_Gameplay_EnemyGetHit>();
	}

	// Set the stats text
	checkSlow(Text_HP);
	Text_HP->SetText(FText::AsNumber(Data.LifeArmorInfo.CurrentLife));
	checkSlow(Text_Armor);
	Text_Armor->SetText(FText::AsNumber(Data.LifeArmorInfo.CurrentArmor));
	checkSlow(Text_Attack);
	Text_Attack->SetText(FText::AsNumber(Data.CauseDamageInfo.InitialDamage));
}

void ASGEnemyTileBase::HandleBeginAttack(const FMessage_Gameplay_EnemyBeginAttack& Message, const IMessageContextRef& Context)
{
	EnemyAttack();
}

void ASGEnemyTileBase::HandlePlayHit(const FMessage_Gameplay_EnemyGetHit& Message, const IMessageContextRef& Context)
{
	FILTER_MESSAGE;
	BeginPlayHit();
}

void ASGEnemyTileBase::BeginPlayHit()
{
	StartPlayHitAnimation();
	
	// Ensure the cached message is valid
	if (CachedDamageMessage.TileID == TileID)
	{
		// Do the take damage now
		if (OnTakeTileDamage(CachedDamageMessage.DamageInfos, Data.LifeArmorInfo) == false)
		{
			// Update the new stats
			checkSlow(Text_HP);
			Text_HP->SetText(FText::AsNumber(Data.LifeArmorInfo.CurrentLife));
			checkSlow(Text_Armor);
			Text_Armor->SetText(FText::AsNumber(Data.LifeArmorInfo.CurrentArmor));
			checkSlow(Text_Attack);
			Text_Attack->SetText(FText::AsNumber(Data.CauseDamageInfo.InitialDamage));
		}
		else
		{
			// Set the dead sprite
			checkSlow(Sprite_Dead);
			checkSlow(GetRenderComponent());
			GetRenderComponent()->SetSprite(Sprite_Dead);
		}
	}
}
