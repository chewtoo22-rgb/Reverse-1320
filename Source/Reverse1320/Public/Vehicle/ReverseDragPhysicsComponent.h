#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle/ReverseVehicleTypes.h"
#include "ReverseDragPhysicsComponent.generated.h"

USTRUCT(BlueprintType)
struct FReverseTimingSlip
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) float ReactionTime = 0.0f;
    UPROPERTY(BlueprintReadOnly) float SixtyFoot = 0.0f;
    UPROPERTY(BlueprintReadOnly) float ThreeThirty = 0.0f;
    UPROPERTY(BlueprintReadOnly) float EighthET = 0.0f;
    UPROPERTY(BlueprintReadOnly) float EighthMPH = 0.0f;
    UPROPERTY(BlueprintReadOnly) float ThousandET = 0.0f;
    UPROPERTY(BlueprintReadOnly) float QuarterET = 0.0f;
    UPROPERTY(BlueprintReadOnly) float QuarterMPH = 0.0f;
};

USTRUCT(BlueprintType)
struct FReverseVehicleRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) float TimeSeconds = 0.0f;
    UPROPERTY(BlueprintReadOnly) float DistanceMeters = 0.0f;
    UPROPERTY(BlueprintReadOnly) float SpeedMetersPerSecond = 0.0f;
    UPROPERTY(BlueprintReadOnly) float EngineRPM = 850.0f;
    UPROPERTY(BlueprintReadOnly) int32 Gear = 1;
    UPROPERTY(BlueprintReadOnly) float WheelSlip = 0.0f;
    UPROPERTY(BlueprintReadOnly) float BoostPsi = 0.0f;
    UPROPERTY(BlueprintReadOnly) bool bNitrousActive = false;
    UPROPERTY(BlueprintReadOnly) bool bFinished = false;
};

UCLASS(ClassGroup=(Reverse1320), meta=(BlueprintSpawnableComponent))
class REVERSE1320_API UReverseDragPhysicsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UReverseDragPhysicsComponent();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Vehicle")
    void Configure(UReverseVehicleDefinition* InVehicle);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Vehicle")
    void ResetRun();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Vehicle")
    void Launch(float Throttle, float TrackGripMultiplier, float ReactionTimeSeconds);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Vehicle")
    void SetThrottle(float Value);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Vehicle")
    bool ShiftUp();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Vehicle")
    void SetNitrous(bool bEnabled);

    UFUNCTION(BlueprintPure, Category="Reverse1320|Vehicle")
    const FReverseVehicleRuntimeState& GetRuntimeState() const { return State; }

    UFUNCTION(BlueprintPure, Category="Reverse1320|Vehicle")
    const FReverseTimingSlip& GetTimingSlip() const { return Slip; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY() TObjectPtr<UReverseVehicleDefinition> Vehicle;
    FReverseVehicleRuntimeState State;
    FReverseTimingSlip Slip;

    float ThrottleInput = 0.0f;
    float TrackGrip = 1.0f;
    float ShiftTimer = 0.0f;
    bool bLaunched = false;

    float SampleTorque(float RPM) const;
    float CalculateBoost(float RPM) const;
    float CalculateDrivenAxleLoad(float AccelerationGuess) const;
    float CalculateMaxTireForce(float AccelerationGuess) const;
    void CaptureSplits(float PreviousDistance, float PreviousTime);
};
