// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallController.h"
#include "Ball/BallCharacter.h"

void ABallController::BeginPlay()
{

	BallCharacter = Cast<ABallCharacter>(GetPawn());

}
