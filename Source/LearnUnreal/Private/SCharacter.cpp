// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
    CameraComp->SetupAttachment(SpringArmComp);

    InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}

void ASCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value)
{
    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.f;
    ControlRot.Roll = 0.f;

    AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.f;
    ControlRot.Roll = 0.f;

    // X is forward, Y is right, Z is up
    FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
    AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
    PlayAnimMontage(AttackAnim);
    GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
    // GetWorldTimerManager().ClearTimer(handle) stops the timer from running when character dies etc.
    // TODO: use animation notify is better, will get to this later.
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
    SpawnProjectile(ProjectileClass);
}

void ASCharacter::BlackHoleAttack()
{
    PlayAnimMontage(AttackAnim);
    GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
    SpawnProjectile(BlackHoleClass);
}

void ASCharacter::Dash()
{
    PlayAnimMontage(AttackAnim);
    GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}

void ASCharacter::Dash_TimeElapsed()
{
    SpawnProjectile(DashProjectileClass);
}

void ASCharacter::PrimaryInteract()
{
    InteractionComp->PrimaryInteract();
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
    if (ensureAlways(ClassToSpawn)) {
        // The muzzle on the right hand of Gideon.
        FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

        FCollisionShape Shape;
        Shape.SetSphere(20.f);

        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        FCollisionObjectQueryParams ObjParams;
        ObjParams.AddObjectTypesToQuery(ECC_WorldStatic); // TODO: When look ahead, the camera could be in the hitbox of landscape, what to do?
        ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
        ObjParams.AddObjectTypesToQuery(ECC_Pawn);

        FVector Start = CameraComp->GetComponentLocation();
        FVector End = Start + GetControlRotation().Vector() * 10000;

        FHitResult Hit;
        if (GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjParams, Shape, Params)) {
            End = Hit.ImpactPoint;
        }

        FRotator ProjRotation = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
        FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Instigator = this;
        AActor* Projectile = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
        GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);
    }
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0) {
        GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
        if (NewHealth <= 0) {
            APlayerController* PC = Cast<APlayerController>(GetController());
            DisableInput(PC);
        }
    }
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
    PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
    PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
    PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}
