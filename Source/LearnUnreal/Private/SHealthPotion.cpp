// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->SetupAttachment(RootComponent);
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
    if (ensure(InstigatorPawn))
    {
        USAttributeComponent* AttrComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (ensure(AttrComp) && !AttrComp->IsFullHealth())
        {
            if (AttrComp->ApplyHealthChange(this, AttrComp->GetHealthMax()))
            {
                HideItemAndCooldown();
            }
        }
    }
}
