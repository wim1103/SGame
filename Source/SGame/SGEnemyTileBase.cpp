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
	bIsAttacking = false;

	bIsPlayingHit = false;
	PlayHitElapsedTime = 0;
	PlayHitDuration = 0.3f;

	Text_Attack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent-Attack"));
	Text_Attack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Text_Armor = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent-Armor"));
	Text_Armor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Text_HP = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent-HP"));
	Text_HP->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASGEnemyTileBase::TickAttacking(float DeltaSeconds)
{
	if (bIsAttacking == false)
	{
		return;
	}

	// Calculate the time ratio
	float Ratio = AttackingElapsedTime / AttackingDuration;
	if (Ratio > 1.0f)
	{
		EndAttack();
		return;
	}

	checkSlow(GetRenderComponent());

	// Scale up the tile
	if (Ratio < AttackingScaleTimeWindow)
	{
		// Calculate the scale up ratio
		float ScaleUpRatio = FMath::Lerp(1.0f, AttackingScaleRatio, Ratio / AttackingScaleTimeWindow);
		GetRenderComponent()->SetWorldScale3D(FVector(ScaleUpRatio, ScaleUpRatio, ScaleUpRatio));
	}
	// Scale down the tile
	else if (Ratio > 1 - AttackingScaleTimeWindow)
	{
		// Calculate the scale down ratio
		float ScaleUpRatio = FMath::Lerp(1.0f, AttackingScaleRatio, (1.0f - Ratio) / AttackingScaleTimeWindow);
		GetRenderComponent()->SetWorldScale3D(FVector(ScaleUpRatio, ScaleUpRatio, ScaleUpRatio));
	}
	// We are in shaking 
	else
	{
		// Calculate the shaking ratio
		float CurrentRatio = (Ratio - AttackingScaleTimeWindow) / (1 - AttackingScaleTimeWindow) * PI * AttackingShakeFreq;

		// Do a sin curve to map to [1, -1]
		float FinalRatio = FMath::Sin(CurrentRatio);
		GetRenderComponent()->SetRelativeRotation(FRotator(FinalRatio * AttackingShakeDegree, 0, 0));
	}

	AttackingElapsedTime += DeltaSeconds;
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

	static bool bUseCodeAttackLogic = false;
	if (bUseCodeAttackLogic == true)
	{
		bIsAttacking = true;
		AttackingElapsedTime = 0;

		// Pop up our tile on top of the fading sprite
		this->AddActorWorldOffset(FVector(0.0f, 1000.0f, 0.0f));

		// Set the attack sprite
		checkSlow(Sprite_Attacking);
		checkSlow(GetRenderComponent());
		GetRenderComponent()->SetSprite(Sprite_Attacking);
	}
	else
	{
		StartAttack();
	}
}

void ASGEnemyTileBase::EndAttack()
{
	// Set the flag, disable the attacking tick
	bIsAttacking = false;

	// Pop down the tile to the original place
	this->AddActorWorldOffset(FVector(0.0f, -1000.0f, 0.0f));

	// Make sure the rotation and scale back to origin
	checkSlow(Sprite_Normal);
	checkSlow(GetRenderComponent());
	GetRenderComponent()->SetSprite(Sprite_Normal);
	GetRenderComponent()->SetRelativeRotation(FRotator(0, 0, 0));
	GetRenderComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void ASGEnemyTileBase::TickPlayHit(float DeltaSeconds)
{
	if (bIsPlayingHit == false)
	{
		return;
	}

	// Calculate the time ratio
	float Ratio = PlayHitElapsedTime / PlayHitDuration;
	if (Ratio > 1.0f)
	{
		EndPlayHit();
		return;
	}

	// Calculate the shaking ratio
	float CurrentRatio = Ratio * PI * AttackingShakeFreq;

	// Do a sin curve to map to [1, -1]
	float FinalRatio = FMath::Sin(CurrentRatio);
	GetRenderComponent()->SetRelativeRotation(FRotator(FinalRatio * AttackingShakeDegree, 0, 0));

	PlayHitElapsedTime += DeltaSeconds;
}

void ASGEnemyTileBase::Tick(float DeltaSeconds)
{
	TickAttacking(DeltaSeconds);
	TickPlayHit(DeltaSeconds);
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
	PlayHitElapsedTime = 0;
	bIsPlayingHit = true;

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

void ASGEnemyTileBase::EndPlayHit()
{
	bIsPlayingHit = false;
}
