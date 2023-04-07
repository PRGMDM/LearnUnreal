// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction_Projectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacter.h"

USAction_Projectile::USAction_Projectile()
{
    MuzzleSocketName = "Muzzle_01";
    AttackAnimDelay = 0.2f;
}

void USAction_Projectile::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    ACharacter* Character = Cast<ACharacter>(InstigatorActor);
    if (Character)
    {
        Character->PlayAnimMontage(AttackAnim);
        UGameplayStatics::SpawnEmitterAttached(AttackVFX, Character->GetMesh(), MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

        if (Character->HasAuthority())
        {
            FTimerHandle TimerHandle_AttackDelay;
            FTimerDelegate Delegate;
            Delegate.BindUFunction(this, "Attack_Elapsed", InstigatorActor);
            // TODO: use animation notify is better, will get to this later.
            GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
        }
    }
}

void USAction_Projectile::Attack_Elapsed(ASCharacter* InstigatorActor)
{
    if (ensureAlways(ProjectileClass))
    {
        // The muzzle on the right hand of Gideon.
        FVector HandLocation = InstigatorActor->GetMesh()->GetSocketLocation(MuzzleSocketName);

        FCollisionShape Shape;
        Shape.SetSphere(20.f);

        FCollisionQueryParams Params;
        Params.AddIgnoredActor(InstigatorActor);

        FCollisionObjectQueryParams ObjParams;
        ObjParams.AddObjectTypesToQuery(ECC_WorldStatic); // TODO: When look above, the camera could be in the hitbox of landscape, what to do? make the trace start further away.
        ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
        ObjParams.AddObjectTypesToQuery(ECC_Pawn);

        FVector Start = InstigatorActor->GetPawnViewLocation();
        FVector End = Start + InstigatorActor->GetControlRotation().Vector() * 10000;

        FHitResult Hit;
        if (GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjParams, Shape, Params))
        {
            End = Hit.ImpactPoint;
        }

        FRotator ProjRotation = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
        FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Instigator = InstigatorActor;
        AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
    }
    StopAction(InstigatorActor);
}
