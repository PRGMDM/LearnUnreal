// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
    RootComponent = MeshComp;

    ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
    ForceComp->SetupAttachment(MeshComp);

    ForceComp->SetAutoActivate(false);
    ForceComp->Radius = 750.f;
    ForceComp->ImpulseStrength = 1000.f;
    ForceComp->bImpulseVelChange = true;

    bReplicates = true;
    // TODO: might need to come back later and adjust the collision channel
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
    Super::BeginPlay();
    MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ForceComp->FireImpulse();
    Destroy();
}
