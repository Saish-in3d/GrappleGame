// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CableComponent.h"




// Sets default values
ABallCharacter::ABallCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
	MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMeshComponent"));
	RootComponent = MainBody;

	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Ball/SM_BallMesh.SM_BallMesh'")); 
	if (MeshAsset.Succeeded())
	{
		MainBody->SetStaticMesh(MeshAsset.Object);
		MainBody->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	GunBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("GunBoom"));
	GunBoom->SetupAttachment(MainBody);
	GunBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	GunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunBody"));
	GunBody->SetupAttachment(GunBoom);



	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(MainBody);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent1"));
	CableComponent->SetHiddenInGame(true);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ABallCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainBody)
	{
		MainBody->OnComponentHit.AddDynamic(this, &ABallCharacter::OnHitGround);
	}
}

void ABallCharacter::PublicJump()
{
	Jump();
}

void ABallCharacter::PublicGrapple()
{
	Grapple();
}

void ABallCharacter::MoveForward(float Value)
{
	if (Value >= 1.f)
	{
		if (MainBody)
		{
			FVector ForceDirTemp = FollowCamera->GetForwardVector() * 1000;
			
			MainBody->AddForce(ForceDirTemp, NAME_None, true);
		}
	}
	else if (Value <= -1.f)
	{
		if (MainBody)
		{
			FVector ForceDirTemp = FollowCamera->GetForwardVector() * -1000;
			ForceDirTemp.Z = 0.f;
			MainBody->AddForce(ForceDirTemp, NAME_None, true);
		}
	}
}

void ABallCharacter::MoveSideWards(float Value)
{
	if (Value >= 1.f)
	{
		if (MainBody)
		{
			FVector ForceDirTemp = FollowCamera->GetRightVector() * 1000;

			MainBody->AddForce(ForceDirTemp, NAME_None, true);
		}
	}
	else if (Value <= -1.f)
	{
		if (MainBody)
		{
			FVector ForceDirTemp = FollowCamera->GetRightVector() * -1000;

			MainBody->AddForce(ForceDirTemp, NAME_None, true);
		}
	}
}

void ABallCharacter::Jump()
{
	if (CanJump == true && MainBody)
	{
		CanJump = false;
		FVector ImpulseDirTemp= FVector(0.f, 0.f, 1000.f);
		MainBody->AddImpulse(ImpulseDirTemp, NAME_None, true);
	}
}

void ABallCharacter::Grapple()
{
	if (IsGrappling == false )
	{
		
		bool retFlag;
		SetGrapple(retFlag);
		if (retFlag) return;

	}
	else if (IsGrappling == true)
	{
		UnsetGrapple();
	}

}

void ABallCharacter::UnsetGrapple()
{
	if (PhysicsConstraintComponentTemp && CableComponent)
	{

		IsGrappling = false;
		PhysicsConstraintComponentTemp->BreakConstraint();
		PhysicsConstraintComponentTemp->DestroyComponent();
		CableComponent->bAttachStart = false;
		CableComponent->SetHiddenInGame(true);
	}
}

void ABallCharacter::SetGrapple(bool& retFlag)
{
	retFlag = true;

	if (FollowCamera == nullptr) { return; }

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	FVector LineTraceStartTemp = FollowCamera->GetComponentLocation();
	FVector LineTraceEndTemp = (FollowCamera->GetForwardVector() * MaxTraceLength + LineTraceStartTemp);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStartTemp, LineTraceEndTemp, ECC_Visibility, CollisionQueryParams))
	{
		FVector ImpactLocationTemp = HitResult.Location;

		CreatePhysicsConstraint(ImpactLocationTemp);
		SetupCable(ImpactLocationTemp);
	}
	retFlag = false;
}

void ABallCharacter::SetupCable(const FVector& ImpactLocationTemp)
{
	if (CableComponent)
	{
		CableComponent->bAttachStart = true;
		CableComponent->SetHiddenInGame(false);
		CableComponent->SetWorldLocation(ImpactLocationTemp);
	}
}

void ABallCharacter::CreatePhysicsConstraint(FVector& ImpactLocationTemp)
{
	FTransform PhysicsConstraintTransform = FTransform();
	PhysicsConstraintTransform.SetLocation(ImpactLocationTemp);

	PhysicsConstraintComponentTemp = Cast<UPhysicsConstraintComponent>(AddComponentByClass(UPhysicsConstraintComponent::StaticClass(), true, PhysicsConstraintTransform, true));

	if (PhysicsConstraintComponentTemp && MainBody)
	{

		IsGrappling = true;

		PhysicsConstraintComponentTemp->SetWorldLocation(ImpactLocationTemp);
		float ConstraintLength = FVector::Distance(MainBody->GetComponentLocation(), ImpactLocationTemp);
		PhysicsConstraintComponentTemp->SetConstrainedComponents(nullptr, FName("Name_None"), MainBody, FName("Name_None"));
		FConstraintInstance ConstraintInstance = PhysicsConstraintComponentTemp->ConstraintInstance;
		ConstraintInstance.SetLinearLimits(ELinearConstraintMotion::LCM_Limited, ELinearConstraintMotion::LCM_Limited, ELinearConstraintMotion::LCM_Limited, ConstraintLength);
	}
}

void ABallCharacter::OnHitGround(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Ground")))
	{
		CanJump = true;
	}

}

// Called every frame
void ABallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ABallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABallCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABallCharacter::MoveSideWards);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABallCharacter::Jump);
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ABallCharacter::Grapple);

}

