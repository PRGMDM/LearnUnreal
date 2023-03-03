// Fill out your copyright notice in the Description page of Project Settings.

#include "SBTService_CheckLowHealth.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    AAIController* MyController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (ensure(MyController) && ensure(BBComp))
    {
        APawn* AIPawn = MyController->GetPawn();
        USAttributeComponent* AttrComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (ensure(AttrComp))
        {
            bool bIsLowHealth = AttrComp->IsLowHealth();
            BBComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
            UE_LOG(LogTemp, Log, TEXT("Is low health: %s"), bIsLowHealth ? TEXT("true") : TEXT("false"));
        }
    }
}
