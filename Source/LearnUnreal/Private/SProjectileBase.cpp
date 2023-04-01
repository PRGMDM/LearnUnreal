// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectileBase.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Projectile");
    RootComponent = SphereComp;

    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetupAttachment(RootComponent);

    MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("MoveComp");
    MoveComp->InitialSpeed = 4000.f;
    MoveComp->ProjectileGravityScale = 0.f;
    MoveComp->bRotationFollowsVelocity = true;
    MoveComp->bInitialVelocityInLocalSpace = true;

    AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
    AudioComp->SetupAttachment(RootComponent);

    bReplicates = true;
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Log, TEXT("Projectile hits %s"), *(OtherComp->GetReadableName()));
    Explode();
}

void ASProjectileBase::Explode_Implementation()
{
    if (ensure(IsValidChecked(this)))
    {
        UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
        Destroy();
    }
}

void ASProjectileBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
    AudioComp->Play();
}
