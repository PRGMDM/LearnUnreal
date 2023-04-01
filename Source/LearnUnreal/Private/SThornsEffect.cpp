// Fill out your copyright notice in the Description page of Project Settings.

#include "SThornsEffect.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USThornsEffect::USThornsEffect()
{
    ActionName = TEXT("Thorns");
    bAutoStart = true;
}

void USThornsEffect::StartAction_Implementation(AActor* Instigator)
{
    Super::StartAction_Implementation(Instigator);
    TObjectPtr<USAttributeComponent> AttrComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
    ensure(AttrComp);
    AttrComp->OnHealthChanged.AddDynamic(this, &USThornsEffect::ReflectDamage);
}

void USThornsEffect::StopAction_Implementation(AActor* Instigator)
{
    Super::StopAction_Implementation(Instigator);
    TObjectPtr<USAttributeComponent> AttrComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
    ensure(AttrComp);
    AttrComp->OnHealthChanged.RemoveDynamic(this, &USThornsEffect::ReflectDamage);
}

void USThornsEffect::ReflectDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{

    TObjectPtr<AActor> OwningActor = GetOwningComponent()->GetOwner();
    if (Delta < 0.f && OwningActor != InstigatorActor)
    {
        USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, -FMath::CeilToInt(-Delta * DamageMultiplier));
    }
}
