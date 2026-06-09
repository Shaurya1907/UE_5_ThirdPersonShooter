// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "ShooterAI.h"

AShooterGameMode::AShooterGameMode()
{
	// stub
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	AShooterCharacter* Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	TArray<AActor*> ShooterAIActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAI::StaticClass(), ShooterAIActors);

	for (int32 LoopIndex = 0; LoopIndex < ShooterAIActors.Num(); LoopIndex++)
	{
		AActor* ShooterAIActor = ShooterAIActors[LoopIndex];

		AShooterAI* ShooterAI = Cast<AShooterAI>(ShooterAIActor);

		if (ShooterAI)
		{
			ShooterAI->StartBehaviorTree(Player);
			UE_LOG(LogTemp, Display, TEXT("%s started behavior tree"), *ShooterAI->GetActorNameOrLabel());
		}
	}
}
