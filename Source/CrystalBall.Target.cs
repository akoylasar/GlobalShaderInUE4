// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

using UnrealBuildTool;
using System.Collections.Generic;

public class CrystalBallTarget : TargetRules
{
	public CrystalBallTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "CrystalBall" } );
	}
}
