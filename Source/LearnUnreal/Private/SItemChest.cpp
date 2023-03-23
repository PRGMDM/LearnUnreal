// Fill out your copyright notice in the Description page of Project Settings.

#include "SItemChest.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
    RootComponent = BaseMesh;
    LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
    LidMesh->SetupAttachment(BaseMesh);

    SetReplicates(true);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
    bLidOpened = !bLidOpened;
    OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
    float CurrPitch = bLidOpened ? TargetPitch : 0;
    LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASItemChest, bLidOpened);
}
