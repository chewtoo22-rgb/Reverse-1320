#include "Vehicle/ReverseRaceVehicleComponent.h"

UReverseRaceVehicleComponent::UReverseRaceVehicleComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 1.0f / 60.0f;
}

void UReverseRaceVehicleComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    DragPhysics = Owner->FindComponentByClass<UReverseDragPhysicsComponent>();
    LaunchDynamics = Owner->FindComponentByClass<UReverseLaunchDynamicsComponent>();
}

void UReverseRaceVehicleComponent::ConfigureVehicle(UReverseVehicleDefinition* InDefinition,
                                                    const FReverseTrackSurfaceData& InTrack)
{
    VehicleDefinition = InDefinition;
    TrackSurface = InTrack;
    Phase = EReverseRaceVehiclePhase::Garage;

    if (DragPhysics)
    {
        DragPhysics->Configure(VehicleDefinition);
    }

    if (LaunchDynamics && VehicleDefinition)
    {
        LaunchDynamics->Configure(VehicleDefinition->DriveTires.Thermal, TrackSurface,
                                  VehicleDefinition->LaunchDevice, VehicleDefinition->WheelieControl);
    }
}

void UReverseRaceVehicleComponent::EnterBurnout()
{
    if (!VehicleDefinition || !LaunchDynamics)
    {
        return;
    }

    Phase = EReverseRaceVehiclePhase::Burnout;
    BurnoutThrottle = 0.0f;
    BurnoutSlip = 0.0f;
}

void UReverseRaceVehicleComponent::SetBurnoutInput(float Throttle, float WheelSlipRatio)
{
    if (Phase != EReverseRaceVehiclePhase::Burnout || !LaunchDynamics)
    {
        return;
    }

    BurnoutThrottle = FMath::Clamp(Throttle, 0.0f, 1.0f);
    BurnoutSlip = FMath::Clamp(WheelSlipRatio, 0.0f, 3.0f);
    LaunchDynamics->SetBurnout(BurnoutThrottle > 0.05f, BurnoutSlip * BurnoutThrottle);
}

void UReverseRaceVehicleComponent::FinishBurnout()
{
    if (Phase != EReverseRaceVehiclePhase::Burnout)
    {
        return;
    }

    if (LaunchDynamics)
    {
        LaunchDynamics->SetBurnout(false, 0.0f);
    }
    BurnoutThrottle = 0.0f;
    BurnoutSlip = 0.0f;
    Phase = EReverseRaceVehiclePhase::PreStage;
}

void UReverseRaceVehicleComponent::PreStage()
{
    if (Phase == EReverseRaceVehiclePhase::Burnout)
    {
        FinishBurnout();
    }

    if (Phase == EReverseRaceVehiclePhase::Garage || Phase == EReverseRaceVehiclePhase::PreStage)
    {
        Phase = EReverseRaceVehiclePhase::PreStage;
    }
}

void UReverseRaceVehicleComponent::Stage()
{
    if (Phase == EReverseRaceVehiclePhase::PreStage)
    {
        Phase = EReverseRaceVehiclePhase::Staged;
    }
}

void UReverseRaceVehicleComponent::ArmTree()
{
    if (Phase == EReverseRaceVehiclePhase::Staged)
    {
        Phase = EReverseRaceVehiclePhase::Tree;
    }
}

bool UReverseRaceVehicleComponent::Launch(float ReactionTimeSeconds, float Throttle)
{
    if (Phase != EReverseRaceVehiclePhase::Tree || !VehicleDefinition || !DragPhysics || !LaunchDynamics)
    {
        return false;
    }

    const float LaunchTorque = LaunchDynamics->BeginLaunch(VehicleDefinition->LaunchDevice.LaunchRPM);
    DragPhysics->SetExternalGripMultiplier(LaunchDynamics->GetEffectiveGripMultiplier());
    DragPhysics->SetExternalTorqueMultiplier(LaunchTorque);
    DragPhysics->Launch(FMath::Clamp(Throttle, 0.0f, 1.0f), 1.0f, ReactionTimeSeconds);
    Phase = EReverseRaceVehiclePhase::Racing;
    return true;
}

bool UReverseRaceVehicleComponent::ShiftUp()
{
    return Phase == EReverseRaceVehiclePhase::Racing && DragPhysics ? DragPhysics->ShiftUp() : false;
}

void UReverseRaceVehicleComponent::SetNitrous(bool bEnabled)
{
    if (DragPhysics && Phase == EReverseRaceVehiclePhase::Racing)
    {
        DragPhysics->SetNitrous(bEnabled);
    }
}

void UReverseRaceVehicleComponent::SetThrottle(float Value)
{
    if (DragPhysics && Phase == EReverseRaceVehiclePhase::Racing)
    {
        DragPhysics->SetThrottle(Value);
    }
}

FReverseTimingSlip UReverseRaceVehicleComponent::GetTimingSlip() const
{
    return DragPhysics ? DragPhysics->GetTimingSlip() : FReverseTimingSlip();
}

FReverseLaunchRuntimeState UReverseRaceVehicleComponent::GetLaunchState() const
{
    return LaunchDynamics ? LaunchDynamics->GetLaunchState() : FReverseLaunchRuntimeState();
}

void UReverseRaceVehicleComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Phase != EReverseRaceVehiclePhase::Racing || !VehicleDefinition || !DragPhysics || !LaunchDynamics)
    {
        return;
    }

    const FReverseVehicleRuntimeState& Runtime = DragPhysics->GetRuntimeState();
    LaunchDynamics->UpdateLaunch(DeltaTime, Runtime.EngineRPM, Runtime.LongitudinalAccelerationMps2,
                                 VehicleDefinition->MassKg, VehicleDefinition->WheelbaseMeters,
                                 VehicleDefinition->CenterOfMassHeightMeters);

    DragPhysics->SetExternalGripMultiplier(LaunchDynamics->GetEffectiveGripMultiplier());
    DragPhysics->SetExternalTorqueMultiplier(LaunchDynamics->GetLaunchTorqueMultiplier());

    if (Runtime.bFinished)
    {
        DragPhysics->SetNitrous(false);
        Phase = EReverseRaceVehiclePhase::Finished;
    }
}
