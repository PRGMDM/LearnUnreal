// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

// Sets default values
USInteractionComponent::USInteractionComponent()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::PrimaryInteract()
{
    UE_LOG(LogTemp, Warning, TEXT("Interaction attempted by character"));
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    AActor* MyOwner = GetOwner();

    // TODO: might want to change the starting point to camera.
    FVector EyeLocation;
    FRotator EyeRotation;
    MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); // Returns the same thing as GetControlRotation in this case.
    FString name;
    MyOwner->GetName(name);

    FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

    // FHitResult Hit;
    // GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

    TArray<FHitResult> Hits;
    FCollisionShape Shape;
    Shape.SetSphere(30.f);
    GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

    for (auto Hit : Hits) {
        AActor* HitActor = Hit.GetActor();
        if (HitActor) {
            if (HitActor->Implements<USGameplayInterface>()) {
                UE_LOG(LogTemp, Warning, TEXT("Interact line trace hit an actor %s, invoking Interact on the actor"), *HitActor->GetName());
                ISGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(MyOwner));
                DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 32, FColor::Red, false, 2.0f);
                break;
            }
        }
    }

    DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Red, false, 2.f, 0, 2.0f);
}

// Called when the game starts or when spawned
void USInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
