// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

using UnrealBuildTool;
using System.Collections.Generic;

public class CrystalBallEditorTarget : TargetRules
{
	public CrystalBallEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "CrystalBall" } );
	}
}
