// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.


#include "BTTask_ClearBlackboardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get the blackboard component
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		// Clear the value of the selected key
		Blackboard->ClearValue(GetSelectedBlackboardKey());
	}

	// Return success
	return EBTNodeResult::Succeeded;

}
