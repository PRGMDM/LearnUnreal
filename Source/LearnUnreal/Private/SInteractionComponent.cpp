// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawing(TEXT("DrawDebug"), false, TEXT("Enable/disable debug drawings"), ECVF_Cheat);

// Sets default values
USInteractionComponent::USInteractionComponent()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::PrimaryInteract()
{
    ServerInteract(FocusedActor);
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    APawn* MyPawn = Cast<APawn>(GetOwner());
    if (MyPawn->IsLocallyControlled())
    {
        FindBestInteractable();
    }
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
    if (InFocus == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("No focused actor to interact with"));
        return;
    }
    APawn* MyPawn = Cast<APawn>(GetOwner());
    ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

void USInteractionComponent::FindBestInteractable()
{
    bool bDrawDebug = CVarDebugDrawing.GetValueOnGameThread();

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

    ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());
    ensureAlways(MyOwner);

    FVector CamLocation = MyOwner->GetPawnViewLocation();
    FRotator EyeRotation = MyOwner->GetControlRotation();

    FVector End = CamLocation + (EyeRotation.Vector() * TraceDistance);

    TArray<FHitResult> Hits;
    FCollisionShape Shape;
    Shape.SetSphere(TraceRadius);
    GetWorld()->SweepMultiByObjectType(Hits, CamLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

    FocusedActor = nullptr;

    for (auto Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            if (HitActor->Implements<USGameplayInterface>())
            {
                FocusedActor = HitActor;
                if (bDrawDebug)
                {
                    DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 32, FColor::Red, false, 2.0f);
                }
                break;
            }
        }
    }

    if (FocusedActor)
    {
        if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
        {
            DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
        }

        if (DefaultWidgetInstance)
        {
            DefaultWidgetInstance->AttachedActor = FocusedActor;
            if (!DefaultWidgetInstance->IsInViewport())
            {
                DefaultWidgetInstance->AddToViewport();
            }
        }
    }
    else if (DefaultWidgetInstance)
    {
        DefaultWidgetInstance->RemoveFromParent();
    }

    if (bDrawDebug)
    {
        DrawDebugLine(GetWorld(), CamLocation, End, FColor::Red, false, 2.f, 0, 2.0f);
    }
}
