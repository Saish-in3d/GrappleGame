// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudWidget.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLEGAME_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	virtual bool Initialize();


	UPROPERTY(meta = (BindWidget))
	class UButton* JumpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GrappleButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;


private:

	UFUNCTION()
	void OnPressedJumpButton();

	UFUNCTION()
	void OnGrappleButton();

	UFUNCTION()
	void OnPressedRestartLevelButton();

	UFUNCTION()
	void OnPressedExitGameButton();


	class ABallCharacter* BallChar;

	FTimerHandle DelayTimerHandle;
	float DelayTime = 2.0f;

	void AfterDlay();

};
