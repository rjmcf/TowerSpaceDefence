// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TowerSpaceDefense.h"
#include "TowerSpaceDefensePawn.h"

ATowerSpaceDefensePawn::ATowerSpaceDefensePawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
    MinSpeed = -MaxSpeed;
	CurrentLocalForwardThrust = 0.f;
    CurrentLocalRightThrust = 0.f;
    CurrentLocalUpThrust = 0.f;
    CurrentGlobalSpeed = FVector(0,0,0);
}

void ATowerSpaceDefensePawn::Tick(float DeltaSeconds)
{
    
    // Vector of local thrusts
    const FVector LocalThrust = FVector(CurrentLocalForwardThrust, CurrentLocalRightThrust, CurrentLocalUpThrust);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
    
    // Get Global Rotation
    const FRotator GlobalRot = GetActorRotation();
    
    // Rotate local thrust to global thrust
    const FVector GlobalThrust = GlobalRot.RotateVector(LocalThrust);
    
    // Update global speed
    CurrentGlobalSpeed += GlobalThrust * DeltaSeconds;
    
    // Calculate global move
    const FVector GlobalMove = CurrentGlobalSpeed * DeltaSeconds;

    // Move plan forwards (with sweep so we stop when we collide with things)
    AddActorWorldOffset(GlobalMove, true);
    
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}

void ATowerSpaceDefensePawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation(RootComponent);
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}


void ATowerSpaceDefensePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	// Bind our control axis' to callback functions
    InputComponent->BindAxis("MoveForward", this, &ATowerSpaceDefensePawn::MoveForwardInput);
    InputComponent->BindAxis("MoveRight", this, &ATowerSpaceDefensePawn::MoveRightInput);
    InputComponent->BindAxis("MoveUp", this, &ATowerSpaceDefensePawn::MoveUpInput);
	InputComponent->BindAxis("LookUp", this, &ATowerSpaceDefensePawn::LookUpInput);
    InputComponent->BindAxis("LookRight", this, &ATowerSpaceDefensePawn::LookRightInput);
    InputComponent->BindAxis("RollRight", this, &ATowerSpaceDefensePawn::RollRightInput);
}

void ATowerSpaceDefensePawn::MoveForwardInput(float Val)
{
	
	// Set Acceleration
	CurrentLocalForwardThrust = (Val * Acceleration);
	// Calculate new speed
	//float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	//CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ATowerSpaceDefensePawn::MoveRightInput(float Val)
{
    // If input is not held down, reduce speed
    CurrentLocalRightThrust = (Val * Acceleration);
    // Calculate new speed
    //float NewRightSpeed = CurrentRightSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
    // Clamp between MinSpeed and MaxSpeed
    //CurrentRightSpeed = FMath::Clamp(NewRightSpeed, MinSpeed, MaxSpeed);
}

void ATowerSpaceDefensePawn::MoveUpInput(float Val)
{
    // If input is not held down, reduce speed
    CurrentLocalUpThrust = (Val * Acceleration);
    // Calculate new speed
    //float NewUpSpeed = CurrentUpSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
    // Clamp between MinSpeed and MaxSpeed
    //CurrentUpSpeed = FMath::Clamp(NewUpSpeed, MinSpeed, MaxSpeed);
}

void ATowerSpaceDefensePawn::LookUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	//TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ATowerSpaceDefensePawn::LookRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	//const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	//float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	//CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ATowerSpaceDefensePawn::RollRightInput(float Val)
{
    // Target roll speed is based in input
    float TargetRollSpeed = (Val * TurnSpeed);
    
    
    // Smoothly interpolate to target roll speed
    CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}
