#pragma once

#include "CoreMinimal.h"
#include "ReverseLaunchTypes.generated.h"

UENUM(BlueprintType)
enum class EReverseLaunchDevice : uint8
{
    DirectClutch,
    TorqueConverter,
    DualClutch
};

USTRUCT(BlueprintType)
struct FReverseTireThermalData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AmbientTemperatureC = 24.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float OptimalTemperatureC = 82.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaximumUsefulTemperatureC = 118.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BurnoutHeatRateCPerSecond = 26.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CoolingRateCPerSecond = 2.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MinimumColdGripMultiplier = 0.72f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaximumHotGripMultiplier = 1.08f;
};

USTRUCT(BlueprintType)
struct FReverseTrackSurfaceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BaseGripMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float PrepLevel = 0.70f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RubberLevel = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float SurfaceTemperatureC = 32.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Moisture = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LaneDegradation = 0.0f;
};

USTRUCT(BlueprintType)
struct FReverseLaunchDeviceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EReverseLaunchDevice Type = EReverseLaunchDevice::TorqueConverter;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LaunchRPM = 3200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float StallRPM = 3400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ClutchBiteTimeSeconds = 0.16f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TorqueMultiplication = 1.70f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LockupSpeedMetersPerSecond = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LaunchTorqueRampSeconds = 0.30f;
};

USTRUCT(BlueprintType)
struct FReverseWheelieControlData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bWheelieBarInstalled = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WheelieBarHeightMeters = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WheelieBarLengthMeters = 1.10f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaximumPitchDegrees = 7.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float FrontLiftThreshold = 0.95f;
};

USTRUCT(BlueprintType)
struct FReverseLaunchRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float DriveTireTemperatureC = 24.0f;

    UPROPERTY(BlueprintReadOnly)
    float EffectiveTrackGrip = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float LaunchTorqueMultiplier = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float EstimatedPitchDegrees = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float FrontAxleLoadFraction = 0.50f;

    UPROPERTY(BlueprintReadOnly)
    bool bBurnoutActive = false;

    UPROPERTY(BlueprintReadOnly)
    bool bWheelieBarContact = false;
};
