// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GrappleGame : ModuleRules
{
	public GrappleGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "CableComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent" });

    }
}
