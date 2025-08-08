// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DdakKong : ModuleRules
{
	public DdakKong(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HTTP", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
