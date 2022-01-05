// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MMXMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UMMXMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UMMXMovementComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	FVector CurrentVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	FVector Acceleration;

	// Horizontal Movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float GroundAcceleration;

	// Vertical Movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	FVector Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float JumpStartSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float JumpStartAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float JumpStopSpeedReduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float JumpStopAccelerationReduction;

	void HorizontalMove(float Value);
	void Jump();
	void StopJump();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void JumpTimerFunc();

	bool IsGrounded;
	FTimerHandle JumpTimerHandle;

	bool HitWall;

	float BoxExtentX;
	float BoxExtentZ;
};
