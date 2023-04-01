// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

    ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    TimeToHitParamName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
    AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* Target)
{
    TObjectPtr<AAIController> AIC = Cast<AAIController>(GetController());
    if (AIC)
    {
        TObjectPtr<UBlackboardComponent> BBComp = AIC->GetBlackboardComponent();

        TObjectPtr<AActor> CurTarget = Cast<AActor>(BBComp->GetValueAsObject("TargetActor"));
        if (CurTarget != Target)
        {
            TObjectPtr<USWorldUserWidget> EnemySpottedWidget = CreateWidget<USWorldUserWidget>(GetWorld(), EnemySpottedWidgetClass);
            EnemySpottedWidget->AttachedActor = this;
            EnemySpottedWidget->AddToViewport(10);
        }
        BBComp->SetValueAsObject("TargetActor", Target); // TODO: Make this like AttackRangeKey in SBTService_CheckAttackRange. Can I?
    }
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
    SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0.f)
    {
        if (InstigatorActor != this)
        {
            UE_LOG(LogTemp, Log, TEXT("Target set"));
            SetTargetActor(InstigatorActor);
        }

        if (!ActiveHealthBar)
        {
            ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
        }

        if (ActiveHealthBar)
        {
            ActiveHealthBar->AttachedActor = this;
            ActiveHealthBar->AddToViewport();
        }

        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

        if (NewHealth <= 0.f)
        {
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }
            GetMesh()->SetAllBodiesSimulatePhysics(true); // ragdoll
            GetMesh()->SetCollisionProfileName("Ragdoll");
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();
            SetLifeSpan(10.f);
        }
    }
}
