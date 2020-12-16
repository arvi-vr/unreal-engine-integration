// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class ARVIIntegration : ModuleRules
{
	public ARVIIntegration(ReadOnlyTargetRules Target) : base(Target)
	{
		//Type = ModuleType.External;
		PublicDefinitions.Add("WITH_ARVIINTEGRATION=1");
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));
		PrivateDefinitions.Add("LIBARVI_STATIC");
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibDir = Path.Combine(ModuleDirectory, "lib");
			LibDir = Path.Combine(LibDir, Target.Platform.ToString());
			LibDir = Path.Combine(LibDir, "VS" + Target.WindowsPlatform.GetVisualStudioCompilerVersionName());
			PublicAdditionalLibraries.Add(Path.Combine(LibDir, "libARVI_s.lib"));
#if UE_4_24_OR_LATER
			PublicSystemLibraries.Add("crypt32.lib");
			PublicSystemLibraries.Add("winhttp.lib");
#else
			PublicAdditionalLibraries.Add("crypt32.lib");
			PublicAdditionalLibraries.Add("winhttp.lib");
#endif
		}

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Json", 
				"JsonUtilities",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
