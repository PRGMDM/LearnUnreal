// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* InstigatorActor, AActor* Target, float DamageAmount)
{
    USAttributeComponent* AttrComp = USAttributeComponent::GetAttributes(Target);
    if (AttrComp)
    {
        return AttrComp->ApplyHealthChange(InstigatorActor, DamageAmount);
    }
    return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* InstigatorActor, AActor* Target, float DamageAmount, const FHitResult& HitResult)
{
    if (ApplyDamage(InstigatorActor, Target, DamageAmount))
    {
        UPrimitiveComponent* HitComp = HitResult.GetComponent();
        if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
        {
            FVector ImpactDir = HitResult.ImpactPoint - HitResult.TraceStart;
            ImpactDir.Normalize();
            HitComp->AddImpulseAtLocation(ImpactDir * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
        }
        return true;
    }
    return false;
}
