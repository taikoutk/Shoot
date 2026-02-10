// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootMyProject : ModuleRules
{
	public ShootMyProject(ReadOnlyTargetRules Target) : base(Target)
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
			"ShootMyProject",
			"ShootMyProject/Variant_Platforming",
			"ShootMyProject/Variant_Platforming/Animation",
			"ShootMyProject/Variant_Combat",
			"ShootMyProject/Variant_Combat/AI",
			"ShootMyProject/Variant_Combat/Animation",
			"ShootMyProject/Variant_Combat/Gameplay",
			"ShootMyProject/Variant_Combat/Interfaces",
			"ShootMyProject/Variant_Combat/UI",
			"ShootMyProject/Variant_SideScrolling",
			"ShootMyProject/Variant_SideScrolling/AI",
			"ShootMyProject/Variant_SideScrolling/Gameplay",
			"ShootMyProject/Variant_SideScrolling/Interfaces",
			"ShootMyProject/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
