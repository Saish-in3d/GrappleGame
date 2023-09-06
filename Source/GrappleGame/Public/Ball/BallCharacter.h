// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallCharacter.generated.h"


UCLASS()
class GRAPPLEGAME_API ABallCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PublicJump();

	void PublicGrapple();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MainBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GunBody;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* GunBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double MaxTraceLength = 10000;


private:

	UFUNCTION()
	void OnHitGround(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
	bool CanJump = true;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveSideWards(float Value);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void Grapple();

	void UnsetGrapple();

	void SetGrapple(bool& retFlag);

	void SetupCable(const FVector& ImpactLocationTemp);

	void CreatePhysicsConstraint(FVector& ImpactLocationTemp);

	bool IsGrappling = false;

	UPROPERTY(VisibleAnywhere)
		class UPhysicsConstraintComponent* PhysicsConstraintComponentTemp;

	UPROPERTY(VisibleAnywhere)
		class UCableComponent* CableComponent;


};
