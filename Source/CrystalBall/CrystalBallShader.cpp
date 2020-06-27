// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#include "CrystalBallShader.h"

#include <Runtime/ShaderCore/Public/ShaderParameterUtils.h>

#include <Runtime/RHI/Public/RHIStaticStates.h>

FCrystalBallVS::FCrystalBallVS()
{
}

FCrystalBallVS::FCrystalBallVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
  : FGlobalShader(Initializer)
{
}

bool FCrystalBallVS::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
  return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}


FCrystalBallPS::FCrystalBallPS()
{
}

FCrystalBallPS::FCrystalBallPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
  : FGlobalShader(Initializer)
{
  CrystalBall.Bind(Initializer.ParameterMap, TEXT("CrystalBall"), EShaderParameterFlags::SPF_Optional);
  IndexOfRefraction.Bind(Initializer.ParameterMap, TEXT("IndexOfRefraction"), EShaderParameterFlags::SPF_Optional);
  FloorTexture.Bind(Initializer.ParameterMap, TEXT("FloorTexture"), EShaderParameterFlags::SPF_Optional);
  FloorTextureSampler.Bind(Initializer.ParameterMap, TEXT("FloorTextureSampler"), EShaderParameterFlags::SPF_Optional);
  Light1.Bind(Initializer.ParameterMap, TEXT("Light1"), EShaderParameterFlags::SPF_Optional);
  Light2.Bind(Initializer.ParameterMap, TEXT("Light2"), EShaderParameterFlags::SPF_Optional);
  AmbientAndShadow.Bind(Initializer.ParameterMap, TEXT("AmbientAndShadow"), EShaderParameterFlags::SPF_Optional);
  CameraPosition.Bind(Initializer.ParameterMap, TEXT("CameraPosition"), EShaderParameterFlags::SPF_Optional);
  AspectRatio.Bind(Initializer.ParameterMap, TEXT("AspectRatio"), EShaderParameterFlags::SPF_Optional);
}

bool FCrystalBallPS::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
  return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

bool FCrystalBallPS::Serialize(FArchive& Ar)
{
  const auto bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
  Ar << CrystalBall;
  Ar << IndexOfRefraction;
  Ar << FloorTexture;
  Ar << FloorTextureSampler;
  Ar << Light1;
  Ar << Light2;
  Ar << AmbientAndShadow;
  Ar << CameraPosition;
  Ar << AspectRatio;
  return bShaderHasOutdatedParameters;
}

void FCrystalBallPS::SetShaderParameters(FRHICommandList& RHICommandList, const FCrystalBallShaderProperties& Properties)
{
  const auto PixelShader = GetPixelShader();
  SetShaderValue(RHICommandList, PixelShader, this->CrystalBall, Properties.CrystalBall);
  SetShaderValue(RHICommandList, PixelShader, this->IndexOfRefraction, Properties.IndexOfRefraction);
  SetTextureParameter(RHICommandList,
                      PixelShader,
                      this->FloorTexture,
                      this->FloorTextureSampler,
                      Properties.FloorTexture->Resource->SamplerStateRHI,
                      Properties.FloorTexture->Resource->TextureRHI);
  SetShaderValue(RHICommandList, PixelShader, this->Light1, Properties.Light1);
  SetShaderValue(RHICommandList, PixelShader, this->Light2, Properties.Light2);
  SetShaderValue(RHICommandList, PixelShader, this->AmbientAndShadow, Properties.AmbientAndShadow);
  SetShaderValue(RHICommandList, PixelShader, this->CameraPosition, Properties.CameraPosition);
  SetShaderValue(RHICommandList, PixelShader, this->AspectRatio, Properties.AspectRatio);
}

IMPLEMENT_SHADER_TYPE(, FCrystalBallVS, TEXT("/Shaders/Main.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(,FCrystalBallPS, TEXT("/Shaders/Main.usf"), TEXT("MainPS"), SF_Pixel);
