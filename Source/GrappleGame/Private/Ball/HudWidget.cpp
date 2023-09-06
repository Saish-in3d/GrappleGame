// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/HudWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/BallCharacter.h"


bool UHudWidget::Initialize()
{
	Super::Initialize();

	JumpButton->OnPressed.AddDynamic(this, &UHudWidget::OnPressedJumpButton);
	GrappleButton->OnPressed.AddDynamic(this, &UHudWidget::OnGrappleButton);
	ExitButton->OnPressed.AddDynamic(this, &UHudWidget::OnPressedExitGameButton);
	RestartButton->OnPressed.AddDynamic(this, &UHudWidget::OnPressedRestartLevelButton);

	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 0 is the index for the first player
	if (LocalPlayerController)
	{
		BallChar = Cast<ABallCharacter>(LocalPlayerController->GetPawn());

	}

	return true;
}

void UHudWidget::OnPressedJumpButton()
{
	if (BallChar)
	{
		BallChar->PublicJump();
	}
}

void UHudWidget::OnGrappleButton()
{
	if (BallChar)
	{
		BallChar->PublicGrapple();
	}
}

void UHudWidget::OnPressedRestartLevelButton()
{
	if(BallChar)
	{
		BallChar->GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &UHudWidget::AfterDlay, DelayTime, false);
	}

	
}

void UHudWidget::OnPressedExitGameButton()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("quit");
	}
}

void UHudWidget::AfterDlay()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"));
}
