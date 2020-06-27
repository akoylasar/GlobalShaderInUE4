// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#include "CrystalBallShader.h"
#include "Properties.generated.h"

USTRUCT()
struct CRYSTALBALL_API FCrystalBall
{
  GENERATED_BODY()

  FCrystalBall()
    : Position(0.0),
      Radius(1.0),
      IndexOfRefraction(2.0)
  {
  }

  void Translate(float X, float Y, float Z)
  {
    Position.X += X;
    Position.Y += Y;
    Position.Z += Z;
  }

  inline void UpdateShaderProperties(FCrystalBallShaderProperties& Properties)
  {
    Properties.CrystalBall.X = Position.X;
    Properties.CrystalBall.Y = Position.Y;
    Properties.CrystalBall.Z = Position.Z;
    Properties.CrystalBall.W = Radius;
    Properties.IndexOfRefraction = IndexOfRefraction;
  }

  UPROPERTY(EditAnywhere)
  FVector Position;
  UPROPERTY(EditAnywhere)
  float Radius;
  UPROPERTY(EditAnywhere)
  float IndexOfRefraction;
};

USTRUCT()
struct CRYSTALBALL_API FSceneLight
{
  GENERATED_BODY()

  FSceneLight()
    : Position(3.0, 3.0, 1.0),
      Intensity(1.0)
  {
  }

  UPROPERTY(EditAnywhere)
  FVector Position;
  UPROPERTY(EditAnywhere)
  float Intensity;
};

USTRUCT()
struct CRYSTALBALL_API FSceneProperties
{
  GENERATED_BODY()

  FSceneProperties()
    : FloorTexture(nullptr),
      AmbientLight(0.15, 0.07, 0.85),
      ShadowFactor(0.5)
  {
  }

  inline void UpdateShaderProperties(FCrystalBallShaderProperties& Properties)
  {
    Properties.FloorTexture = FloorTexture;
    Properties.AmbientAndShadow.X = AmbientLight.X;
    Properties.AmbientAndShadow.Y = AmbientLight.Y;
    Properties.AmbientAndShadow.Z = AmbientLight.Z;
    Properties.AmbientAndShadow.W = ShadowFactor;
  }

  UPROPERTY(EditAnywhere)
  UTexture2D* FloorTexture;
  UPROPERTY(EditAnywhere)
  FVector AmbientLight;
  UPROPERTY(EditAnywhere)
  float ShadowFactor;
};
