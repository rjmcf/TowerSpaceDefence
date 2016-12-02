// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "TowerSpaceDefensePawn.generated.h"

UCLASS(config=Game)
class ATowerSpaceDefensePawn : public APawn
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
    
    /** Planet reference for gravity */
    UPROPERTY(Category = Planet, EditAnywhere, meta = (AllowPrivateAccess = "true"))
    class AActor* Planet;
    
    /** Bullet reference */
    UPROPERTY(Category = Firing, EditAnywhere, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AActor> ProjectileClass;
    
public:
	ATowerSpaceDefensePawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

protected:

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the forward axis */
	void MoveForwardInput(float Val);
    
    /** Bound to the right axis */
    void MoveRightInput(float Val);
    
    /** Bound to the up axis */
    void MoveUpInput(float Val);
	
	/** Bound to the vertical axis */
	void LookUpInput(float Val);

	/** Bound to the horizontal axis */
	void LookRightInput(float Val);
    
    /** Bound to the roll axis */
    void RollRightInput(float Val);
    
    /* For spawning bullets */
    void spawnBullet();

private:

	/** How quickly forward speed changes */
	UPROPERTY(Category=Plane, EditAnywhere)
	float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category=Plane, EditAnywhere)
	float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category=Yaw, EditAnywhere)
	float MinSpeed;

	/** Current forward speed */
	float CurrentLocalForwardThrust;
    
    /** Current right speed */
    float CurrentLocalRightThrust;
    
    /** Current up speed */
    float CurrentLocalUpThrust;
    
    /** Current global speed */
    FVector CurrentGlobalSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;
    
    /** Gravity Strength */
    float Gravity;
    
    /** Period until bullet spawn */
    int Countdown;

public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
