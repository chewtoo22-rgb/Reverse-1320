#include "Vehicle/ReverseDragPhysicsComponent.h"

namespace Reverse1320Physics
{
    constexpr float AirDensity = 1.225f;
    constexpr float Gravity = 9.80665f;
    constexpr float MeterToMph = 2.23693629f;
    constexpr float QuarterMileMeters = 402.336f;
    constexpr float EighthMileMeters = 201.168f;
    constexpr float SixtyFootMeters = 18.288f;
    constexpr float ThreeThirtyFeetMeters = 100.584f;
    constexpr float ThousandFeetMeters = 304.8f;
}

UReverseDragPhysicsComponent::UReverseDragPhysicsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 1.0f / 120.0f;
}

void UReverseDragPhysicsComponent::BeginPlay()
{
    Super::BeginPlay();
    ResetRun();
}

void UReverseDragPhysicsComponent::Configure(UReverseVehicleDefinition* InVehicle)
{
    Vehicle = InVehicle;
    ResetRun();
}

void UReverseDragPhysicsComponent::ResetRun()
{
    State = FReverseVehicleRuntimeState();
    Slip = FReverseTimingSlip();
    ThrottleInput = 0.0f;
    TrackGrip = 1.0f;
    ExternalGripMultiplier = 1.0f;
    ExternalTorqueMultiplier = 1.0f;
    ShiftTimer = 0.0f;
    bLaunched = false;

    if (Vehicle)
    {
        State.EngineRPM = Vehicle->IdleRPM;
    }
}

void UReverseDragPhysicsComponent::Launch(float Throttle, float TrackGripMultiplier, float ReactionTimeSeconds)
{
    if (!Vehicle || bLaunched)
    {
        return;
    }

    ThrottleInput = FMath::Clamp(Throttle, 0.0f, 1.0f);
    TrackGrip = FMath::Max(0.1f, TrackGripMultiplier);
    Slip.ReactionTime = ReactionTimeSeconds;
    bLaunched = true;
}

void UReverseDragPhysicsComponent::SetThrottle(float Value)
{
    ThrottleInput = FMath::Clamp(Value, 0.0f, 1.0f);
}

bool UReverseDragPhysicsComponent::ShiftUp()
{
    if (!Vehicle || ShiftTimer > 0.0f || State.Gear >= Vehicle->Gearbox.GearRatios.Num())
    {
        return false;
    }

    ++State.Gear;
    ShiftTimer = FMath::Max(0.02f, Vehicle->Gearbox.ShiftTimeSeconds);
    return true;
}

void UReverseDragPhysicsComponent::SetNitrous(bool bEnabled)
{
    State.bNitrousActive = bEnabled && Vehicle && Vehicle->NitrousTorqueMultiplier > 1.0f;
}

void UReverseDragPhysicsComponent::SetExternalGripMultiplier(float Value)
{
    ExternalGripMultiplier = FMath::Clamp(Value, 0.25f, 2.0f);
}

void UReverseDragPhysicsComponent::SetExternalTorqueMultiplier(float Value)
{
    ExternalTorqueMultiplier = FMath::Clamp(Value, 0.25f, 3.0f);
}

float UReverseDragPhysicsComponent::SampleTorque(float RPM) const
{
    if (!Vehicle || Vehicle->TorqueCurve.Num() == 0)
    {
        return 0.0f;
    }

    if (RPM <= Vehicle->TorqueCurve[0].RPM)
    {
        return Vehicle->TorqueCurve[0].TorqueNm;
    }

    for (int32 Index = 1; Index < Vehicle->TorqueCurve.Num(); ++Index)
    {
        const FReverseEnginePoint& A = Vehicle->TorqueCurve[Index - 1];
        const FReverseEnginePoint& B = Vehicle->TorqueCurve[Index];
        if (RPM <= B.RPM)
        {
            const float Alpha = FMath::GetRangePct(A.RPM, B.RPM, RPM);
            return FMath::Lerp(A.TorqueNm, B.TorqueNm, Alpha);
        }
    }

    return Vehicle->TorqueCurve.Last().TorqueNm;
}

float UReverseDragPhysicsComponent::CalculateBoost(float RPM) const
{
    if (!Vehicle || Vehicle->Induction == EReverseInduction::NaturallyAspirated || Vehicle->PeakBoostPsi <= 0.0f)
    {
        return 0.0f;
    }

    if (Vehicle->Induction == EReverseInduction::Supercharged)
    {
        return Vehicle->PeakBoostPsi * FMath::Clamp(RPM / Vehicle->RedlineRPM, 0.25f, 1.0f);
    }

    const float SpoolRange = FMath::Max(500.0f, Vehicle->RedlineRPM - Vehicle->BoostThresholdRPM);
    const float Spool = FMath::Clamp((RPM - Vehicle->BoostThresholdRPM) / SpoolRange, 0.0f, 1.0f);
    return Vehicle->PeakBoostPsi * FMath::InterpEaseInOut(0.0f, 1.0f, Spool, 2.0f);
}

float UReverseDragPhysicsComponent::CalculateDrivenAxleLoad(float AccelerationGuess) const
{
    if (!Vehicle)
    {
        return 0.0f;
    }

    const float StaticTotal = Vehicle->MassKg * Reverse1320Physics::Gravity;
    const float Transfer = Vehicle->MassKg * AccelerationGuess * Vehicle->CenterOfMassHeightMeters /
        FMath::Max(0.5f, Vehicle->WheelbaseMeters);

    switch (Vehicle->DriveLayout)
    {
        case EReverseDriveLayout::FWD:
            return FMath::Max(0.0f, StaticTotal * 0.60f - Transfer);
        case EReverseDriveLayout::AWD:
            return StaticTotal;
        case EReverseDriveLayout::RWD:
        default:
            return FMath::Max(0.0f, StaticTotal * 0.50f + Transfer);
    }
}

float UReverseDragPhysicsComponent::CalculateMaxTireForce(float AccelerationGuess) const
{
    if (!Vehicle)
    {
        return 0.0f;
    }

    return CalculateDrivenAxleLoad(AccelerationGuess) * Vehicle->DriveTires.BaseGripCoefficient *
        TrackGrip * ExternalGripMultiplier;
}

void UReverseDragPhysicsComponent::CaptureSplits(float PreviousDistance, float PreviousTime)
{
    const auto Crossed = [&](float TargetMeters) { return PreviousDistance < TargetMeters && State.DistanceMeters >= TargetMeters; };
    const auto InterpolatedTime = [&](float TargetMeters)
    {
        const float Segment = State.DistanceMeters - PreviousDistance;
        const float Alpha = Segment > KINDA_SMALL_NUMBER ? (TargetMeters - PreviousDistance) / Segment : 1.0f;
        return FMath::Lerp(PreviousTime, State.TimeSeconds, FMath::Clamp(Alpha, 0.0f, 1.0f));
    };

    if (Slip.SixtyFoot <= 0.0f && Crossed(Reverse1320Physics::SixtyFootMeters))
        Slip.SixtyFoot = InterpolatedTime(Reverse1320Physics::SixtyFootMeters);

    if (Slip.ThreeThirty <= 0.0f && Crossed(Reverse1320Physics::ThreeThirtyFeetMeters))
        Slip.ThreeThirty = InterpolatedTime(Reverse1320Physics::ThreeThirtyFeetMeters);

    if (Slip.EighthET <= 0.0f && Crossed(Reverse1320Physics::EighthMileMeters))
    {
        Slip.EighthET = InterpolatedTime(Reverse1320Physics::EighthMileMeters);
        Slip.EighthMPH = State.SpeedMetersPerSecond * Reverse1320Physics::MeterToMph;
    }

    if (Slip.ThousandET <= 0.0f && Crossed(Reverse1320Physics::ThousandFeetMeters))
        Slip.ThousandET = InterpolatedTime(Reverse1320Physics::ThousandFeetMeters);

    if (!State.bFinished && Crossed(Reverse1320Physics::QuarterMileMeters))
    {
        Slip.QuarterET = InterpolatedTime(Reverse1320Physics::QuarterMileMeters);
        Slip.QuarterMPH = State.SpeedMetersPerSecond * Reverse1320Physics::MeterToMph;
        State.bFinished = true;
    }
}

void UReverseDragPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!Vehicle || !bLaunched || State.bFinished)
    {
        return;
    }

    const float Dt = FMath::Min(DeltaTime, 1.0f / 30.0f);
    const float PreviousDistance = State.DistanceMeters;
    const float PreviousTime = State.TimeSeconds;

    ShiftTimer = FMath::Max(0.0f, ShiftTimer - Dt);
    const int32 GearIndex = FMath::Clamp(State.Gear - 1, 0, Vehicle->Gearbox.GearRatios.Num() - 1);
    const float GearRatio = Vehicle->Gearbox.GearRatios.IsValidIndex(GearIndex) ? Vehicle->Gearbox.GearRatios[GearIndex] : 1.0f;

    const float WheelAngularSpeed = State.SpeedMetersPerSecond / FMath::Max(0.05f, Vehicle->DriveTires.RadiusMeters);
    const float CalculatedRPM = WheelAngularSpeed * GearRatio * Vehicle->Gearbox.FinalDrive * 60.0f / (2.0f * PI);
    State.EngineRPM = FMath::Clamp(FMath::Max(CalculatedRPM, Vehicle->IdleRPM), Vehicle->IdleRPM, Vehicle->RedlineRPM * 1.05f);
    State.BoostPsi = CalculateBoost(State.EngineRPM);

    float EngineTorque = SampleTorque(State.EngineRPM) * ThrottleInput;
    const float BoostMultiplier = 1.0f + (State.BoostPsi / 14.7f);
    EngineTorque *= BoostMultiplier;

    if (State.bNitrousActive)
    {
        EngineTorque *= Vehicle->NitrousTorqueMultiplier;
    }

    EngineTorque *= ExternalTorqueMultiplier;

    if (ShiftTimer > 0.0f)
    {
        EngineTorque *= 0.12f;
    }

    const float WheelTorque = EngineTorque * GearRatio * Vehicle->Gearbox.FinalDrive * Vehicle->DrivetrainEfficiency;
    const float RequestedDriveForce = WheelTorque / FMath::Max(0.05f, Vehicle->DriveTires.RadiusMeters);

    const float AccelerationGuess = RequestedDriveForce / FMath::Max(1.0f, Vehicle->MassKg);
    const float TireLimit = CalculateMaxTireForce(AccelerationGuess);
    const float DriveForce = FMath::Min(RequestedDriveForce, TireLimit);
    State.WheelSlip = RequestedDriveForce > KINDA_SMALL_NUMBER ?
        FMath::Max(0.0f, RequestedDriveForce / FMath::Max(1.0f, TireLimit) - 1.0f) : 0.0f;

    const float SpeedSq = State.SpeedMetersPerSecond * State.SpeedMetersPerSecond;
    const float AeroDrag = 0.5f * Reverse1320Physics::AirDensity * Vehicle->DragCoefficient * Vehicle->FrontalAreaSquareMeters * SpeedSq;
    const float RollingResistance = Vehicle->DriveTires.RollingResistanceCoefficient * Vehicle->MassKg * Reverse1320Physics::Gravity;
    const float NetForce = DriveForce - AeroDrag - RollingResistance;
    const float Acceleration = NetForce / FMath::Max(1.0f, Vehicle->MassKg);
    State.LongitudinalAccelerationMps2 = Acceleration;

    State.SpeedMetersPerSecond = FMath::Max(0.0f, State.SpeedMetersPerSecond + Acceleration * Dt);
    State.DistanceMeters += State.SpeedMetersPerSecond * Dt;
    State.TimeSeconds += Dt;

    CaptureSplits(PreviousDistance, PreviousTime);
}
