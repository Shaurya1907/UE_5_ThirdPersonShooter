// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UBTTaskNode_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_Shoot();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
