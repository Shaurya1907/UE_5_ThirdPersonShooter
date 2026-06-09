// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "ShooterCharacter.h"

#include "ShooterAI.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterAI : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyAIBehaviorTree;

	AShooterCharacter* PlayerCharacter;
	AShooterCharacter* MyCharacter;

	void StartBehaviorTree(AShooterCharacter* Player);

};
