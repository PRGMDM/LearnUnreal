// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTTask_Heal.h"
#include "AI/SAIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* MyController = OwnerComp.GetAIOwner();
    if (ensure(MyController))
    {
        APawn* AIPawn = MyController->GetPawn();
        USAttributeComponent* AttrComp = USAttributeComponent::GetAttributes(AIPawn);
        if (ensure(AttrComp))
        {
            return AttrComp->ApplyHealthChange(AIPawn, AttrComp->GetHealthMax()) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
        }
    }
    return EBTNodeResult::Failed;
}
