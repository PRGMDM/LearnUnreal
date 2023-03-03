// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTService_CheckAttackRange.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (ensure(BlackboardComp))
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
        if (TargetActor)
        {
            AAIController* MyController = OwnerComp.GetAIOwner();
            if (ensure(MyController))
            {
                APawn* AIPawn = MyController->GetPawn();
                if (ensure(AIPawn))
                {
                    float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
                    bool bWithinRange = Distance < 2000.f;
                    bool bHasLOS = false;
                    if (bWithinRange)
                    {
                        bHasLOS = MyController->LineOfSightTo(TargetActor);
                    }
                    BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
                }
            }
        }
        /*else
        {
            UE_LOG(LogTemp, Warning, TEXT("No target"));
        }*/
    }
}
