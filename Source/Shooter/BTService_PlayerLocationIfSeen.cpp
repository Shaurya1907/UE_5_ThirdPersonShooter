// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.


#include "BTService_PlayerLocationIfSeen.h"

#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update PlayerLocation If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	AShooterCharacter* Player = OwnerController->PlayerCharacter;
	UBlackboardComponent* Blackboard = OwnerController->GetBlackboardComponent();

	if(OwnerController && Player && Blackboard)
	{
		if(OwnerController->LineOfSightTo(Player))
		{
			Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Player->K2_GetActorLocation());
			OwnerController->SetFocus(Player);
		}
		else
		{
			Blackboard->ClearValue(GetSelectedBlackboardKey());
			OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}
