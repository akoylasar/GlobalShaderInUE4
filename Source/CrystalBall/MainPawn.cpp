// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#include "MainPawn.h"

#include <Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <Runtime/Engine/Classes/Components/StaticMeshComponent.h>

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>

#include "RenderingUtilities.h"

// Sets default values
AMainPawn::AMainPawn()
{
	PrimaryActorTick.bCanEverTick = true;

  AutoPossessPlayer = EAutoReceiveInput::Player0;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AMainPawn::BeginPlay()
{
	Super::BeginPlay();

  check(RenderTarget);
  check(SceneProperties.FloorTexture);
  check(ScreenWidgetResource);
  
  AutoPossessPlayer = EAutoReceiveInput::Player0;

  UWorld* World = GetWorld();
  PlayerController = World->GetFirstPlayerController();
  check(PlayerController);

  UGameInstance* GameInstance = GetGameInstance();
  
  ScreenWidget = CreateWidget<UUserWidget>(GameInstance, ScreenWidgetResource);
  check(ScreenWidget);
  ScreenWidget->AddToViewport();
}

void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
 
  CrystalBallProperties.UpdateShaderProperties(CrystalBallShaderProperties);
  
  SceneProperties.UpdateShaderProperties(CrystalBallShaderProperties);

  // Update aspect ratio.
  UWorld* World = GetWorld();
  FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(World);
  CrystalBallShaderProperties.AspectRatio = ViewPortSize.X / ViewPortSize.Y;
  
  // Animate lights.
  UpdateLights(World->GetTimeSeconds());

  UpdateCamera();

  // Render the scene.
  RenderingUtilities::RenderSceneToRenderTarget(World, CrystalBallShaderProperties, RenderTarget);
}

void AMainPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

  InputComponent->BindAction("MouseLeft", IE_Pressed, this, &AMainPawn::OnMousePressed);
  InputComponent->BindAction("MouseLeft", IE_Released, this, &AMainPawn::OnMouseReleased);

  InputComponent->BindAxis("MoveX", this, &AMainPawn::OnMoveX);
  InputComponent->BindAxis("MoveY", this, &AMainPawn::OnMoveY);
  InputComponent->BindAxis("MoveZ", this, &AMainPawn::OnMoveZ);

  InputComponent->BindAxis("UpdateRadius", this, &AMainPawn::OnUpdateRadius);

  InputComponent->BindAxis("UpdateIor", this, &AMainPawn::OnUpdateIor);
}

void AMainPawn::UpdateLights(float Time)
{
  if (bAnimateLights)
  {
    float Del = AnimationSpeed * Time;
    float R = OffsetFromSphere + CrystalBallProperties.Radius;

    float Sin = R * FMath::Sin(Del);
    float Cos = R * FMath::Cos(Del);
    
    FVector SC = FVector(Sin, 0.0, Cos);
    FVector CS = FVector(Cos, 0.0, Sin);
    
    Light1.Position = SC + CrystalBallProperties.Position;
    Light2.Position = CS + CrystalBallProperties.Position;

    Light1.Position.Y = Light1Height;
    Light2.Position.Y = Light2Height;
  }

  CrystalBallShaderProperties.Light1.X = Light1.Position.X;
  CrystalBallShaderProperties.Light1.Y = Light1.Position.Y;
  CrystalBallShaderProperties.Light1.Z = Light1.Position.Z;
  CrystalBallShaderProperties.Light1.W = Light1.Intensity;

  CrystalBallShaderProperties.Light2.X = Light2.Position.X;
  CrystalBallShaderProperties.Light2.Y = Light2.Position.Y;
  CrystalBallShaderProperties.Light2.Z = Light2.Position.Z;
  CrystalBallShaderProperties.Light2.W = Light2.Intensity;
}

void AMainPawn::UpdateCamera()
{
  // Update camera position (normalized by viewport size).
  if (bMousePressed)
  {
    float MouseDeltaX, MouseDeltaY;
    PlayerController->GetInputMouseDelta(MouseDeltaX, MouseDeltaY);

    // Calculate the radius (of the sphere camera position is located on).
    float R = CameraPosition.Size();
    float R2 = R * R;

    // Calculate the projected (XZ plane) radius.
    float Rp = FMath::Sqrt(R2 - CameraPosition.Y * CameraPosition.Y);
    
    // Calculate the azimuth angle.
    float Azimuth = FMath::Acos(CameraPosition.X / Rp);
    Azimuth = CameraPosition.Z < 0.0 ? 2.0 * PI - Azimuth : Azimuth;

    // Calculate the elevation angle.
    float Elevation = FMath::Asin(CameraPosition.Y / R);

    // Increase the elevation angle by adding delta of the vertical mouse movement.
    constexpr float MinElevation = PI * 0.05;
    constexpr float MaxElevation = PI * 0.45;
    float NewElevation = FMath::Clamp(Elevation + 0.1f * MouseDeltaY, MinElevation, MaxElevation);

    // Calculate the new Y value.
    float NewY = R * FMath::Sin(NewElevation);

    // Calculate the new size of the projected radius.
    float NewRp = FMath::Sqrt(R2 - NewY * NewY);

    // Calculate new azimuth.
    float NewAzimuth = Azimuth + 0.1f * MouseDeltaX;
    
    // Cacluate new X and Z values.
    float NewX = NewRp * FMath::Cos(NewAzimuth);
    float NewZ = NewRp * FMath::Sin(NewAzimuth);

    CameraPosition = FVector(NewX, NewY, NewZ);
  }
  CrystalBallShaderProperties.CameraPosition = CameraPosition;
}

void AMainPawn::OnMousePressed()
{
  bMousePressed = true;
}

void AMainPawn::OnMouseReleased()
{
  bMousePressed = false;
}

void AMainPawn::OnMoveX(float Value)
{
  CrystalBallProperties.Position.X += UpdateRateMultiplier * Value;
}

void AMainPawn::OnMoveY(float Value)
{
  CrystalBallProperties.Position.Y += UpdateRateMultiplier * Value;
}

void AMainPawn::OnMoveZ(float Value)
{
  CrystalBallProperties.Position.Z += UpdateRateMultiplier * Value;
}

void AMainPawn::OnUpdateRadius(float Value)
{
  CrystalBallProperties.Radius += UpdateRateMultiplier * Value;
}

void AMainPawn::OnUpdateIor(float Value)
{
  CrystalBallProperties.IndexOfRefraction += UpdateRateMultiplier * Value;
  CrystalBallProperties.IndexOfRefraction = FMath::Clamp(CrystalBallProperties.IndexOfRefraction, 1.05f, 3.0f);
}


