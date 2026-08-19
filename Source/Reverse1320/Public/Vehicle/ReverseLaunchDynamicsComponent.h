#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle/ReverseLaunchTypes.h"
#include "ReverseLaunchDynamicsComponent.generated.h"

UCLASS(ClassGroup=(Reverse1320), meta=(BlueprintSpawnableComponent))
class REVERSE1320_API UReverseLaunchDynamicsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UReverseLaunchDynamicsComponent();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Launch")
    void Configure(const FReverseTireThermalData& InTireThermals,
                   const FReverseTrackSurfaceData& InTrack,
                   const FReverseLaunchDeviceData& InLaunchDevice,
                   const FReverseWheelieControlData& InWheelieControl);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Launch")
    void ResetLaunchState();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Launch")
    void SetBurnout(bool bEnabled, float WheelSlipRatio);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Launch")
    void SetTrackSurface(const FReverseTrackSurfaceData& InTrack);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Launch")
    float BeginLaunch(float EngineRPM);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Launch")
    void UpdateLaunch(float DeltaTime, float EngineRPM, float LongitudinalAccelerationMps2,
                      float MassKg, float WheelbaseMeters, float CenterOfMassHeightMeters);

    UFUNCTION(BlueprintPure, Category="Reverse1320|Launch")
    float GetEffectiveGripMultiplier() const { return State.EffectiveTrackGrip; }

    UFUNCTION(BlueprintPure, Category="Reverse1320|Launch")
    float GetLaunchTorqueMultiplier() const { return State.LaunchTorqueMultiplier; }

    UFUNCTION(BlueprintPure, Category="Reverse1320|Launch")
    const FReverseLaunchRuntimeState& GetLaunchState() const { return State; }

protected:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

private:
    FReverseTireThermalData TireThermals;
    FReverseTrackSurfaceData Track;
    FReverseLaunchDeviceData LaunchDevice;
    FReverseWheelieControlData WheelieControl;
    FReverseLaunchRuntimeState State;

    float BurnoutSlipRatio = 0.0f;
    float LaunchElapsed = 0.0f;
    bool bLaunchActive = false;

    float CalculateTireTemperatureGrip() const;
    float CalculateSurfaceGrip() const;
    float CalculateDeviceTorqueMultiplier(float EngineRPM) const;
};
