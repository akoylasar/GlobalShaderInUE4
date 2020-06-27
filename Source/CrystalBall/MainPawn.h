// Copyright (c) Fouad Valadbeigi (akoylasar@gmail.com)

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include <Runtime/Engine/Classes/GameFramework/Pawn.h>


#include "CrystalBallShader.h"
#include "Properties.h"
#include "MainPawn.generated.h"

UCLASS()
class CRYSTALBALL_API AMainPawn : public APawn
{
	GENERATED_BODY()

public:
	AMainPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
  void UpdateLights(float Time);
  void UpdateCamera();

  void OnMousePressed();
  void OnMouseReleased();

  void OnMoveX(float Value);
  void OnMoveY(float Value);
  void OnMoveZ(float Value);

  void OnUpdateRadius(float Value);

  void OnUpdateIor(float Value);

private:
  UPROPERTY(EditDefaultsOnly)
  UTextureRenderTarget2D* RenderTarget;
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<class UUserWidget> ScreenWidgetResource;
  UUserWidget* ScreenWidget;
  APlayerController* PlayerController;

  FCrystalBallShaderProperties CrystalBallShaderProperties;
  UPROPERTY(EditAnywhere)
  FCrystalBall CrystalBallProperties;
  UPROPERTY(EditAnywhere)
  FSceneLight Light1;
  UPROPERTY(EditAnywhere)
  FSceneLight Light2;
  UPROPERTY(EditAnywhere)
  FSceneProperties SceneProperties;

  UPROPERTY(VisibleDefaultsOnly)
  FVector CameraPosition;
  bool bMousePressed;

  UPROPERTY(EditAnywhere)
  float UpdateRateMultiplier = 1.0;

  UPROPERTY(EditAnywhere)
  bool bAnimateLights = true;
  UPROPERTY(EditAnywhere)
  float AnimationSpeed = 1.0;
  UPROPERTY(EditAnywhere)
  float OffsetFromSphere = 2.0;
  UPROPERTY(EditAnywhere)
  float Light1Height = 2.0;
  UPROPERTY(EditAnywhere)
  float Light2Height = 3.0;
};
