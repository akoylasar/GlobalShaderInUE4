// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#include "RenderingUtilities.h"

#include <Runtime/Engine/Classes/Engine/World.h>
#include <Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h>

#include <Runtime/Engine/Public/SceneInterface.h>
#include <Runtime/Engine/Public/SceneManagement.h>

#include <Runtime/RHI/Public/RHIDefinitions.h>
#include <Runtime/RHI/Public/PipelineStateCache.h>

#include <Runtime/RenderCore/Public/RenderingThread.h>

#include <Runtime/ShaderCore/Public/GlobalShader.h>

#include <Runtime/UtilityShaders/Public/ClearQuad.h>

#include "CrystalBallShader.h"

namespace
{
  static void RenderSceneToRenderTarget_RenderThread(const FCrystalBallShaderProperties& SceneProperties,
                                                     FTextureRenderTargetResource* const DestinationRenderTarget,
                                                     ERHIFeatureLevel::Type FeatureLevel,
                                                     FRHICommandListImmediate& RHICommandList)
  {
    check(IsInRenderingThread());
    SCOPED_DRAW_EVENT(RHICommandList, RenderSceneToRenderTarget_RenderThread);

    auto GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
    TShaderMapRef<FCrystalBallVS> VS(GlobalShaderMap);
    TShaderMapRef<FCrystalBallPS> PS(GlobalShaderMap);

    // Set render target.
    SetRenderTarget(RHICommandList,
                    DestinationRenderTarget->GetRenderTargetTexture(),
                    FTextureRHIRef(),
                    ESimpleRenderTargetMode::EUninitializedColorAndDepth,
                    FExclusiveDepthStencil::DepthNop_StencilNop);

    // Set viewport.
    RHICommandList.SetViewport(0, 0, 0.0f, DestinationRenderTarget->GetSizeX(), DestinationRenderTarget->GetSizeY(), 1.f);

    // Set graphics pipeline state.
    FGraphicsPipelineStateInitializer GraphicsPSOInit;
    RHICommandList.ApplyCachedRenderTargets(GraphicsPSOInit);
    GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI(); // No depth write, depth test always passes.
    GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
    GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI(); // Default rasterizer settings. MSAA enabled. TODO: Play with these settings if time allows.
    GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;
    // NOTE: Here we define vertex attributes.
    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GetVertexDeclarationFVector4();
    GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VS);
    GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PS);
    SetGraphicsPipelineState(RHICommandList, GraphicsPSOInit);

    PS->SetShaderParameters(RHICommandList, SceneProperties);

    // Render.
    // NOTE: The quad used here and in those place makse use of GlobalResource (it uses static data so it's good for things like quads and fixed geometry in general).
    RHICommandList.SetStreamSource(0, GClearVertexBuffer.VertexBufferRHI, 0);
    RHICommandList.DrawPrimitive(PT_TriangleStrip, 0, 2, 1);
  }
}

CRYSTALBALL_API void RenderingUtilities::ClearRenderTarget(const UWorld* const World,
                                                           UTextureRenderTarget2D* const DestinationRenderTarget,
                                                           FLinearColor ClearColor)
{
  if (DestinationRenderTarget
      && DestinationRenderTarget->Resource
      && World)
  {
    FTextureRenderTargetResource* RenderTargetResource = DestinationRenderTarget->GameThread_GetRenderTargetResource();
    ENQUEUE_RENDER_COMMAND(ClearRTCommand)([RenderTargetResource, ClearColor](FRHICommandList& RHICmdList)
    {
      SetRenderTarget(RHICmdList, RenderTargetResource->GetRenderTargetTexture(), FTextureRHIRef(), true);
      DrawClearQuad(RHICmdList, ClearColor);
    });
  }
}

CRYSTALBALL_API void RenderingUtilities::RenderSceneToRenderTarget(const UWorld* const World,
                                                                   const FCrystalBallShaderProperties& Properties,
                                                                   UTextureRenderTarget2D* const DestinationRenderTarget)
{
  check(IsInGameThread());

  ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
  FTextureRenderTargetResource* DestinationRenderTargetResource = DestinationRenderTarget->GameThread_GetRenderTargetResource();

  ENQUEUE_RENDER_COMMAND(CrystalBallRenderCommand)([&Properties, FeatureLevel, DestinationRenderTargetResource](FRHICommandListImmediate& RHICommandList)
  {
    RenderSceneToRenderTarget_RenderThread(Properties,
                                           DestinationRenderTargetResource,
                                           FeatureLevel,
                                           RHICommandList);
  });
}
