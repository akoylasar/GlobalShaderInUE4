// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#pragma once

#include <Runtime/ShaderCore/Public/GlobalShader.h>

#include <Runtime/Engine/Classes/Engine/Texture2D.h>

struct CRYSTALBALL_API FCrystalBallShaderProperties
{
  FVector4 CrystalBall;
  FVector4 Light1;
  FVector4 Light2;
  FVector4 AmbientAndShadow;
  const UTexture2D* FloorTexture;
  FVector CameraPosition;
  float IndexOfRefraction;
  float AspectRatio;

  FCrystalBallShaderProperties()
    : CrystalBall(0.0, 2.0, 0.0, 2.0),
      Light1(3.0, 15.0, -3.0, 3.6),
      Light2(-3.0, 10.0, 3.0, 1.0),
      AmbientAndShadow(0.15, 0.07, 0.85, 0.15),
      FloorTexture(nullptr),
      CameraPosition(3.0, 6.0, 9.0),
      IndexOfRefraction(1.18),
      AspectRatio(1.0)
  {
  }
};

class CRYSTALBALL_API FCrystalBallVS : public FGlobalShader
{
  DECLARE_SHADER_TYPE(FCrystalBallVS, Global, /**/);
public:
  FCrystalBallVS();
  FCrystalBallVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

  static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
};

class CRYSTALBALL_API FCrystalBallPS : public FGlobalShader
{
  DECLARE_SHADER_TYPE(FCrystalBallPS, Global, /**/);
public:
  FCrystalBallPS();
  FCrystalBallPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

  static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);

  bool Serialize(FArchive& Ar) override;

  void SetShaderParameters(FRHICommandList& RHICommandList, const FCrystalBallShaderProperties& Properties);

private:
  FShaderParameter CrystalBall;
  FShaderParameter IndexOfRefraction;
  FShaderResourceParameter FloorTexture;
  FShaderResourceParameter FloorTextureSampler;
  FShaderParameter Light1;
  FShaderParameter Light2;
  FShaderParameter AmbientAndShadow;
  FShaderParameter CameraPosition;
  FShaderParameter AspectRatio;
};
