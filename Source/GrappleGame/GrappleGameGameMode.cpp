// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrappleGameGameMode.h"
#include "GrappleGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGrappleGameGameMode::AGrappleGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
