// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASHealthPotion::ASHealthPotion()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->SetupAttachment(RootComponent);
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
    TObjectPtr<ASCharacter> Player = Cast<ASCharacter>(InstigatorPawn);
    if (ensure(Player))
    {
        TObjectPtr<ASPlayerState> State = Player->GetPlayerState<ASPlayerState>();
        if (ensure(State) && !(State->GetCredits() >= Cost))
        {
            FString DebugMsg = FString::Printf(TEXT("Not enough credits for potion, need: %f have: %f"), Cost, State->GetCredits());
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMsg);
            return;
        }

        USAttributeComponent* AttrComp = USAttributeComponent::GetAttributes(InstigatorPawn);
        if (ensure(AttrComp) && !AttrComp->IsFullHealth())
        {
            if (AttrComp->ApplyHealthChange(this, AttrComp->GetHealthMax()))
            {
                State->ModifyCredit(-Cost);
                HideItemAndCooldown();
            }
        }
    }
}

FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
    USAttributeComponent* AttrComp = USAttributeComponent::GetAttributes(InstigatorPawn);
    if (ensure(AttrComp) && AttrComp->IsFullHealth())
    {
        return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
    }

    return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits."), Cost);
}

#undef LOCTEXT_NAMESPACE
