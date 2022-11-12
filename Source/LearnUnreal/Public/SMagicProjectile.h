// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class LEARNUNREAL_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	UParticleSystemComponent* EffectComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// TODO: Maybe ditch the hit event and go for the overlap event and then decide what to do with the projectile.
// i.e. go through if the other actor is owner/teammate and explode/deal damage and destroy self in other cases.

// TODO: Maybe add a lifespan, destroy self if go too far/after some time.