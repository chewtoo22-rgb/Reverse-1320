#include "Testing/ReversePhase4TestActor.h"
#include "Engine/Engine.h"

AReversePhase4TestActor::AReversePhase4TestActor()
{
    PrimaryActorTick.bCanEverTick = true;

    DragPhysics = CreateDefaultSubobject<UReverseDragPhysicsComponent>(TEXT("DragPhysics"));
    LaunchDynamics = CreateDefaultSubobject<UReverseLaunchDynamicsComponent>(TEXT("LaunchDynamics"));
    RaceVehicle = CreateDefaultSubobject<UReverseRaceVehicleComponent>(TEXT("RaceVehicle"));
}

void AReversePhase4TestActor::BeginPlay()
{
    Super::BeginPlay();

    BuildTransientTestVehicle();

    FReverseTrackSurfaceData Track;
    Track.BaseGripMultiplier = 1.0f;
    Track.PrepLevel = 0.90f;
    Track.RubberLevel = 0.80f;
    Track.SurfaceTemperatureC = 34.0f;
    Track.Moisture = 0.0f;
    Track.LaneDegradation = 0.0f;

    RaceVehicle->ConfigureVehicle(TestVehicle, Track);
    RaceVehicle->EnterBurnout();
    RaceVehicle->SetBurnoutInput(0.92f, 1.4f);

    UE_LOG(LogTemp, Display, TEXT("Reverse1320 Phase4 device sequence: burnout started"));
}

void AReversePhase4TestActor::BuildTransientTestVehicle()
{
    TestVehicle = NewObject<UReverseVehicleDefinition>(this, TEXT("Phase4TransientVehicle"));
    TestVehicle->VehicleId = TEXT("phase4_ci_vehicle");
    TestVehicle->DisplayName = FText::FromString(TEXT("Phase 4 Device Test Vehicle"));
    TestVehicle->DriveLayout = EReverseDriveLayout::RWD;
    TestVehicle->MassKg = 1600.0f;
    TestVehicle->WheelbaseMeters = 2.85f;
    TestVehicle->CenterOfMassHeightMeters = 0.55f;
    TestVehicle->DragCoefficient = 0.34f;
    TestVehicle->FrontalAreaSquareMeters = 2.15f;
    TestVehicle->IdleRPM = 900.0f;
    TestVehicle->RedlineRPM = 7800.0f;
    TestVehicle->DrivetrainEfficiency = 0.86f;
    TestVehicle->PeakBoostPsi = 0.0f;
    TestVehicle->NitrousTorqueMultiplier = 1.18f;

    TestVehicle->TorqueCurve = {
        {1000.0f, 420.0f}, {2000.0f, 500.0f}, {3000.0f, 560.0f}, {4000.0f, 590.0f},
        {5000.0f, 600.0f}, {6000.0f, 585.0f}, {7000.0f, 545.0f}, {7800.0f, 485.0f}
    };

    TestVehicle->Gearbox.GearRatios = {2.66f, 1.78f, 1.30f, 1.00f, 0.74f, 0.50f};
    TestVehicle->Gearbox.FinalDrive = 3.55f;
    TestVehicle->Gearbox.ShiftTimeSeconds = 0.16f;

    TestVehicle->DriveTires.RadiusMeters = 0.335f;
    TestVehicle->DriveTires.BaseGripCoefficient = 1.25f;
    TestVehicle->DriveTires.RollingResistanceCoefficient = 0.014f;
    TestVehicle->DriveTires.Thermal.AmbientTemperatureC = 24.0f;
    TestVehicle->DriveTires.Thermal.OptimalTemperatureC = 82.0f;
    TestVehicle->DriveTires.Thermal.MaximumUsefulTemperatureC = 118.0f;

    TestVehicle->LaunchDevice.Type = EReverseLaunchDevice::TorqueConverter;
    TestVehicle->LaunchDevice.LaunchRPM = 3200.0f;
    TestVehicle->LaunchDevice.StallRPM = 3400.0f;
    TestVehicle->LaunchDevice.TorqueMultiplication = 1.55f;
    TestVehicle->LaunchDevice.LaunchTorqueRampSeconds = 0.30f;

    TestVehicle->WheelieControl.bWheelieBarInstalled = false;
    TestVehicle->WheelieControl.MaximumPitchDegrees = 7.5f;
}

void AReversePhase4TestActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    RunAutomatedSequence(DeltaSeconds);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1320, 0.0f, FColor::Green, GetStatusText());
    }
}

void AReversePhase4TestActor::RunAutomatedSequence(float DeltaSeconds)
{
    PhaseClock += DeltaSeconds;

    const EReverseRaceVehiclePhase Phase = RaceVehicle->GetPhase();

    if (Phase == EReverseRaceVehiclePhase::Burnout && PhaseClock >= 2.25f)
    {
        RaceVehicle->FinishBurnout();
        RaceVehicle->PreStage();
        RaceVehicle->Stage();
        RaceVehicle->ArmTree();
        RaceVehicle->Launch(0.165f, 1.0f);
        PhaseClock = 0.0f;
        UE_LOG(LogTemp, Display, TEXT("Reverse1320 Phase4 device sequence: launched"));
        return;
    }

    if (Phase == EReverseRaceVehiclePhase::Racing)
    {
        const FReverseVehicleRuntimeState Runtime = RaceVehicle->GetRuntimeState();

        if (Runtime.EngineRPM >= 7100.0f && Runtime.Gear > LastShiftedGear)
        {
            LastShiftedGear = Runtime.Gear;
        }

        if (Runtime.EngineRPM >= 7100.0f && Runtime.Gear == LastShiftedGear)
        {
            if (RaceVehicle->ShiftUp())
            {
                ++LastShiftedGear;
            }
        }

        RaceVehicle->SetNitrous(Runtime.TimeSeconds >= 1.5f && Runtime.TimeSeconds <= 5.0f);
    }

    if (Phase == EReverseRaceVehiclePhase::Finished && !bReportedFinish)
    {
        bReportedFinish = true;
        const FReverseTimingSlip Slip = RaceVehicle->GetTimingSlip();
        UE_LOG(LogTemp, Display,
            TEXT("REVERSE1320_PHASE4_RESULT RT=%.3f 60=%.3f 330=%.3f 1/8=%.3f@%.2f 1000=%.3f 1/4=%.3f@%.2f"),
            Slip.ReactionTime, Slip.SixtyFoot, Slip.ThreeThirty, Slip.EighthET, Slip.EighthMPH,
            Slip.ThousandET, Slip.QuarterET, Slip.QuarterMPH);
    }
}

FString AReversePhase4TestActor::GetStatusText() const
{
    if (!RaceVehicle)
    {
        return TEXT("Reverse 1320 Phase 4 - no race vehicle");
    }

    const FReverseVehicleRuntimeState Runtime = RaceVehicle->GetRuntimeState();
    const FReverseLaunchRuntimeState Launch = RaceVehicle->GetLaunchState();
    const FReverseTimingSlip Slip = RaceVehicle->GetTimingSlip();

    return FString::Printf(
        TEXT("Reverse 1320 Phase 4\nPhase %d | %.1f mph | %.0f rpm | G%d\nTire %.1f C | Grip %.3f | Slip %.2f\n60 %.3f | 1/8 %.3f | 1/4 %.3f @ %.1f"),
        static_cast<int32>(RaceVehicle->GetPhase()),
        Runtime.SpeedMetersPerSecond * 2.2369363f,
        Runtime.EngineRPM,
        Runtime.Gear,
        Launch.DriveTireTemperatureC,
        Launch.EffectiveTrackGrip,
        Runtime.WheelSlip,
        Slip.SixtyFoot,
        Slip.EighthET,
        Slip.QuarterET,
        Slip.QuarterMPH);
}
