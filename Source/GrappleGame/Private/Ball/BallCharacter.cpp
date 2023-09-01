// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ABallCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ABallCharacter::MoveForward(float Value)
{
	if (Value == 1.f)
	{
		if (MainBody)
		{
			FVector ForceDirTemp = FollowCamera->GetForwardVector() * 1000;
			
			MainBody->AddForce(ForceDirTemp, NAME_None, true);
		}
	}
	else if (Value == -1.f)
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
	if (Value == 1.f)
	{
		if (MainBody)
		{
			FVector ForceDirTemp = FollowCamera->GetRightVector() * 1000;

			MainBody->AddForce(ForceDirTemp, NAME_None, true);
		}
	}
	else if (Value == -1.f)
	{
		if (MainBody)
		{
			FVector ForceDirTemp = FollowCamera->GetRightVector() * -1000;

			MainBody->AddForce(ForceDirTemp, NAME_None, true);
		}
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

}

