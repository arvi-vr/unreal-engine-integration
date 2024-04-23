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

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibDir = Path.Combine(ModuleDirectory, "lib");
			LibDir = Path.Combine(LibDir, Target.Platform.ToString());
			switch (Target.WindowsPlatform.Compiler) {
#if !UE_5_1_OR_LATER
				case WindowsCompiler.VisualStudio2017:
					LibDir = Path.Combine(LibDir, "VS2017");
					break;
#endif
#if !UE_5_4_OR_LATER
				case WindowsCompiler.VisualStudio2019:
					LibDir = Path.Combine(LibDir, "VS2019");
					break;
#endif
				case WindowsCompiler.VisualStudio2022:
					LibDir = Path.Combine(LibDir, "VS2022");
					break;
				default:
					throw new Exception("Unsupported compiler version " + Target.WindowsPlatform.Compiler);
			}
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
