// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class PROTOTYPE_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	FVector CurrentVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
	FVector Acceleration;

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

private:

	void HorizontalMove(float Value);
	void Jump();
	void StopJump();
	void JumpTimerFunc();

	bool IsGrounded;
	FTimerHandle JumpTimerHandle;
};
