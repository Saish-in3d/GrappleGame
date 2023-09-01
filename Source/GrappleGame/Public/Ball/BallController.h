// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BallController.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLEGAME_API ABallController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	class ABallCharacter* BallCharacter;
};
