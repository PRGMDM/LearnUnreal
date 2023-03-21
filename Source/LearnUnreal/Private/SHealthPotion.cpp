// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"

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
