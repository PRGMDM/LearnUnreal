// Fill out your copyright notice in the Description page of Project Settings.

#include "SCoin.h"
#include "SCharacter.h"
#include "SPlayerState.h"

ASCoin::ASCoin()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->SetupAttachment(RootComponent);
}

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
    TObjectPtr<ASCharacter> Player = Cast<ASCharacter>(InstigatorPawn);
    if (Player)
    {
        TObjectPtr<ASPlayerState> State = Player->GetPlayerState<ASPlayerState>();
        ensure(State);
        State->ModifyCredit(GrantCredits);
        HideItemAndCooldown();
    }
}
