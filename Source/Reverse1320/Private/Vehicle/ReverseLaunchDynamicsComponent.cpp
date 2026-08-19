#include "Vehicle/ReverseLaunchDynamicsComponent.h"

namespace Reverse1320Launch
{
    constexpr float Gravity = 9.80665f;
}

UReverseLaunchDynamicsComponent::UReverseLaunchDynamicsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 1.0f / 60.0f;
}

void UReverseLaunchDynamicsComponent::Configure(const FReverseTireThermalData& InTireThermals,
                                                const FReverseTrackSurfaceData& InTrack,
                                                const FReverseLaunchDeviceData& InLaunchDevice,
                                                const FReverseWheelieControlData& InWheelieControl)
{
    TireThermals = InTireThermals;
    Track = InTrack;
    LaunchDevice = InLaunchDevice;
    WheelieControl = InWheelieControl;
    ResetLaunchState();
}

void UReverseLaunchDynamicsComponent::ResetLaunchState()
{
    State = FReverseLaunchRuntimeState();
    State.DriveTireTemperatureC = TireThermals.AmbientTemperatureC;
    State.EffectiveTrackGrip = CalculateSurfaceGrip() * CalculateTireTemperatureGrip();
    State.LaunchTorqueMultiplier = 1.0f;
    State.FrontAxleLoadFraction = 0.50f;
    BurnoutSlipRatio = 0.0f;
    LaunchElapsed = 0.0f;
    bLaunchActive = false;
}

void UReverseLaunchDynamicsComponent::SetBurnout(bool bEnabled, float WheelSlipRatio)
{
    State.bBurnoutActive = bEnabled;
    BurnoutSlipRatio = FMath::Clamp(WheelSlipRatio, 0.0f, 3.0f);
}

void UReverseLaunchDynamicsComponent::SetTrackSurface(const FReverseTrackSurfaceData& InTrack)
{
    Track = InTrack;
    State.EffectiveTrackGrip = CalculateSurfaceGrip() * CalculateTireTemperatureGrip();
}

float UReverseLaunchDynamicsComponent::BeginLaunch(float EngineRPM)
{
    bLaunchActive = true;
    LaunchElapsed = 0.0f;
    State.bBurnoutActive = false;
    BurnoutSlipRatio = 0.0f;
    State.LaunchTorqueMultiplier = CalculateDeviceTorqueMultiplier(EngineRPM);
    return State.LaunchTorqueMultiplier;
}

void UReverseLaunchDynamicsComponent::UpdateLaunch(float DeltaTime, float EngineRPM,
                                                   float LongitudinalAccelerationMps2, float MassKg,
                                                   float WheelbaseMeters, float CenterOfMassHeightMeters)
{
    if (!bLaunchActive)
    {
        return;
    }

    LaunchElapsed += FMath::Max(0.0f, DeltaTime);

    const float Ramp = LaunchDevice.LaunchTorqueRampSeconds > KINDA_SMALL_NUMBER
        ? FMath::Clamp(LaunchElapsed / LaunchDevice.LaunchTorqueRampSeconds, 0.0f, 1.0f)
        : 1.0f;
    const float DeviceMultiplier = CalculateDeviceTorqueMultiplier(EngineRPM);
    State.LaunchTorqueMultiplier = FMath::Lerp(1.0f, DeviceMultiplier, Ramp);

    const float SafeWheelbase = FMath::Max(0.5f, WheelbaseMeters);
    const float TransferFraction = LongitudinalAccelerationMps2 * CenterOfMassHeightMeters /
        (Reverse1320Launch::Gravity * SafeWheelbase);
    State.FrontAxleLoadFraction = FMath::Clamp(0.50f - TransferFraction, 0.0f, 0.75f);

    const float LiftDemand = FMath::Clamp((0.18f - State.FrontAxleLoadFraction) / 0.18f, 0.0f, 1.0f);
    State.EstimatedPitchDegrees = LiftDemand * 14.0f;

    State.bWheelieBarContact = WheelieControl.bWheelieBarInstalled &&
        State.EstimatedPitchDegrees >= WheelieControl.MaximumPitchDegrees;

    if (State.bWheelieBarContact)
    {
        State.EstimatedPitchDegrees = WheelieControl.MaximumPitchDegrees;
        // Bar contact keeps the launch from unloading the front axle completely.
        State.FrontAxleLoadFraction = FMath::Max(State.FrontAxleLoadFraction, 0.08f);
        State.LaunchTorqueMultiplier *= 0.985f;
    }

    State.EffectiveTrackGrip = CalculateSurfaceGrip() * CalculateTireTemperatureGrip();

    (void)MassKg; // Reserved for sprung/unsprung pitch model in the next fidelity step.
}

float UReverseLaunchDynamicsComponent::CalculateTireTemperatureGrip() const
{
    const float T = State.DriveTireTemperatureC;
    const float Ambient = TireThermals.AmbientTemperatureC;
    const float Optimal = FMath::Max(Ambient + 1.0f, TireThermals.OptimalTemperatureC);
    const float MaxUseful = FMath::Max(Optimal + 1.0f, TireThermals.MaximumUsefulTemperatureC);

    if (T <= Optimal)
    {
        const float Alpha = FMath::Clamp((T - Ambient) / (Optimal - Ambient), 0.0f, 1.0f);
        return FMath::Lerp(TireThermals.MinimumColdGripMultiplier,
                           TireThermals.MaximumHotGripMultiplier, Alpha);
    }

    const float Overheat = FMath::Clamp((T - Optimal) / (MaxUseful - Optimal), 0.0f, 1.0f);
    return FMath::Lerp(TireThermals.MaximumHotGripMultiplier, 0.78f, Overheat);
}

float UReverseLaunchDynamicsComponent::CalculateSurfaceGrip() const
{
    const float Prep = FMath::Clamp(Track.PrepLevel, 0.0f, 1.0f);
    const float Rubber = FMath::Clamp(Track.RubberLevel, 0.0f, 1.0f);
    const float MoisturePenalty = FMath::Lerp(1.0f, 0.42f, FMath::Clamp(Track.Moisture, 0.0f, 1.0f));
    const float DegradationPenalty = FMath::Lerp(1.0f, 0.80f, FMath::Clamp(Track.LaneDegradation, 0.0f, 1.0f));
    const float SurfaceTempBonus = FMath::Clamp(1.0f + (Track.SurfaceTemperatureC - 25.0f) * 0.0025f, 0.92f, 1.08f);
    return Track.BaseGripMultiplier * FMath::Lerp(0.82f, 1.12f, Prep) *
        FMath::Lerp(0.92f, 1.06f, Rubber) * MoisturePenalty * DegradationPenalty * SurfaceTempBonus;
}

float UReverseLaunchDynamicsComponent::CalculateDeviceTorqueMultiplier(float EngineRPM) const
{
    switch (LaunchDevice.Type)
    {
        case EReverseLaunchDevice::TorqueConverter:
        {
            const float StallProximity = 1.0f - FMath::Clamp(
                FMath::Abs(EngineRPM - LaunchDevice.StallRPM) / FMath::Max(500.0f, LaunchDevice.StallRPM),
                0.0f, 1.0f);
            return FMath::Lerp(1.0f, FMath::Max(1.0f, LaunchDevice.TorqueMultiplication), StallProximity);
        }
        case EReverseLaunchDevice::DualClutch:
            return FMath::Clamp(1.10f + LaunchDevice.ClutchBiteTimeSeconds * 0.20f, 1.0f, 1.25f);
        case EReverseLaunchDevice::DirectClutch:
        default:
            return 1.0f;
    }
}

void UReverseLaunchDynamicsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float Dt = FMath::Max(0.0f, DeltaTime);
    if (State.bBurnoutActive)
    {
        const float Heat = TireThermals.BurnoutHeatRateCPerSecond * FMath::Clamp(BurnoutSlipRatio, 0.15f, 2.5f) * Dt;
        State.DriveTireTemperatureC = FMath::Min(TireThermals.MaximumUsefulTemperatureC + 30.0f,
                                                 State.DriveTireTemperatureC + Heat);
    }
    else
    {
        const float Cooling = TireThermals.CoolingRateCPerSecond * Dt;
        State.DriveTireTemperatureC = FMath::Max(TireThermals.AmbientTemperatureC,
                                                 State.DriveTireTemperatureC - Cooling);
    }

    State.EffectiveTrackGrip = CalculateSurfaceGrip() * CalculateTireTemperatureGrip();
}
