// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#pragma once

class UWorld;
class UTextureRenderTarget2D;
struct FLinearColor;

struct FCrystalBallShaderProperties;

class CRYSTALBALL_API RenderingUtilities
{
public:
  static void ClearRenderTarget(const UWorld* const World,
                                UTextureRenderTarget2D* const DestinationRenderTarget,
                                FLinearColor ClearColor);

   static void RenderSceneToRenderTarget(const UWorld* const World,
                                         const FCrystalBallShaderProperties& Properties,
                                         UTextureRenderTarget2D* const DestinationRenderTarget);
};
