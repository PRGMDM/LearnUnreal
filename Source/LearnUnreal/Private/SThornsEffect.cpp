// Fill out your copyright notice in the Description page of Project Settings.

#include "SThornsEffect.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"

USThornsEffect::USThornsEffect()
{
    ActionName = TEXT("Thorns");
    bAutoStart = true;
}

void USThornsEffect::StartAction_Implementation(AActor* Instigator)
{
    Super::StartAction_Implementation(Instigator);
    TObjectPtr<USActionComponent> ActionComp = GetOwningComponent();
    ensure(ActionComp);
    TObjectPtr<AActor> MyCharacter = ActionComp->GetOwner();
    ensure(MyCharacter);
    TObjectPtr<USAttributeComponent> AttrComp = USAttributeComponent::GetAttributes(MyCharacter);
    ensure(AttrComp);
    AttrComp->OnHealthChanged.AddDynamic(this, &USThornsEffect::ReflectDamage);
}

void USThornsEffect::StopAction_Implementation(AActor* Instigator)
{
    Super::StopAction_Implementation(Instigator);
    TObjectPtr<USActionComponent> ActionComp = GetOwningComponent();
    ensure(ActionComp);
    TObjectPtr<AActor> MyCharacter = ActionComp->GetOwner();
    ensure(MyCharacter);
    TObjectPtr<USAttributeComponent> AttrComp = USAttributeComponent::GetAttributes(MyCharacter);
    ensure(AttrComp);
    AttrComp->OnHealthChanged.RemoveDynamic(this, &USThornsEffect::ReflectDamage);
}

void USThornsEffect::ReflectDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    TObjectPtr<USAttributeComponent> AttrComp = USAttributeComponent::GetAttributes(InstigatorActor);
    ensure(AttrComp);
    AttrComp->ApplyHealthChange(GetOwningComponent()->GetOwner(), FMath::CeilToInt(Delta * DamageMultiplier));
    UE_LOG(LogTemp, Log, TEXT("Reflecting %f dmg to %s"), Delta * DamageMultiplier, *GetNameSafe(InstigatorActor));
}
