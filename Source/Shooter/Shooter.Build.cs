// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Shooter : ModuleRules
{
	public Shooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Shooter",
			"Shooter/Variant_Platforming",
			"Shooter/Variant_Platforming/Animation",
			"Shooter/Variant_Combat",
			"Shooter/Variant_Combat/AI",
			"Shooter/Variant_Combat/Animation",
			"Shooter/Variant_Combat/Gameplay",
			"Shooter/Variant_Combat/Interfaces",
			"Shooter/Variant_Combat/UI",
			"Shooter/Variant_SideScrolling",
			"Shooter/Variant_SideScrolling/AI",
			"Shooter/Variant_SideScrolling/Gameplay",
			"Shooter/Variant_SideScrolling/Interfaces",
			"Shooter/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
