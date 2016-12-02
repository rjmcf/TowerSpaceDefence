// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

UCLASS()
class TOWERSPACEDEFENSE_API AProjectileActor : public AActor
{
	GENERATED_BODY()
private:
    UPROPERTY()
    UStaticMeshComponent *mSMC;
	
public:
	// Sets default values for this actor's properties
	AProjectileActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
