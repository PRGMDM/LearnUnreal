// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

    // Could setup collision manually as well.
    // SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
    // SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    // SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    // This profile was created in project settings
    SphereComp->SetCollisionProfileName("Projectile");
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap); // TODO: Maybe switch on hit as suggested by documentation
    RootComponent = SphereComp;

    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetupAttachment(SphereComp);

    MovementComp = CreateAbstractDefaultSubobject<UProjectileMovementComponent>("MovementComp");
    MovementComp->InitialSpeed = 4000.f;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor) {
        USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (AttributeComp) {
            AttributeComp->ApplyHealthChange(-20.f);
            Destroy();
        }
    }
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
