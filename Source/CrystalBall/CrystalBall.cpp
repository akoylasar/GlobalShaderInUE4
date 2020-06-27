// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#include "CrystalBall.h"

#include <Runtime/Core/Public/Modules/ModuleManager.h>

#include <Runtime/ShaderCore/Public/ShaderCore.h>

void CrystalBallGameModule::StartupModule()
{
  const TMap<FString, FString>& DirectoryMapping = AllShaderSourceDirectoryMappings();
  const FString VirtualDirectory = "/Shaders";
  if (!DirectoryMapping.Contains(VirtualDirectory))
  {
    AddShaderSourceDirectoryMapping(VirtualDirectory, FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders")));
  }
}

IMPLEMENT_PRIMARY_GAME_MODULE(CrystalBallGameModule, CrystalBall, "CrystalBall");
