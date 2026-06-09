// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.


#include "ShooterAI.h"

#include "Kismet/GameplayStatics.h"

void AShooterAI::BeginPlay()
{
	Super::BeginPlay();

}

void AShooterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* 
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		if (LineOfSightTo(PlayerPawn))
		{
			SetFocus(PlayerPawn);
			MoveToActor(PlayerPawn, 200.0f);
		}
		else
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			StopMovement();
		}
	}*/

}

void AShooterAI::StartBehaviorTree(AShooterCharacter* Player)
{

	if (EnemyAIBehaviorTree)
	{
		MyCharacter = Cast<AShooterCharacter>(GetPawn());

		if (Player)
		{
			PlayerCharacter = Player;
		}

		RunBehaviorTree(EnemyAIBehaviorTree);
	}
}
