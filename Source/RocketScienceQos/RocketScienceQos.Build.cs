 /*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

using UnrealBuildTool;

public class RocketScienceQos : ModuleRules
{
	public RocketScienceQos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"HTTP",
				"Json",
				"JsonUtilities",
				"Icmp"
            }
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{	
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Sockets",
                "Networking"
            }
			);
	}
}
