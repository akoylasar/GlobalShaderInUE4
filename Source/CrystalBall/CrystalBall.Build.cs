// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

using UnrealBuildTool;

public class CrystalBall : ModuleRules
{
	public CrystalBall(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Adding ShaderCore. Refer to CrystalBall.cpp.
        // Adding RHI and RenderCore because the global shaders in CrystalBall.cpp need it.
        // Adding UtilityShaders because CystalBallRenderer makes use of DrawClearQuad utility function provided by this module.
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ShaderCore", "RHI", "RenderCore", "UtilityShaders", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
