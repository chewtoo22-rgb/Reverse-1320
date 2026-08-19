#include "Input/ReverseRaceInputComponent.h"

UReverseRaceInputComponent::UReverseRaceInputComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UReverseRaceInputComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!RaceVehicle)
    {
        if (AActor* Owner = GetOwner())
        {
            RaceVehicle = Owner->FindComponentByClass<UReverseRaceVehicleComponent>();
        }
    }
}

void UReverseRaceInputComponent::BindRaceVehicle(UReverseRaceVehicleComponent* InRaceVehicle)
{
    RaceVehicle = InRaceVehicle;
}

void UReverseRaceInputComponent::SetThrottleAxis(float Value)
{
    ThrottleAxis = FMath::Clamp(Value, 0.0f, 1.0f);

    if (!RaceVehicle)
    {
        return;
    }

    if (RaceVehicle->GetPhase() == EReverseRaceVehiclePhase::Burnout)
    {
        RaceVehicle->SetBurnoutInput(ThrottleAxis, BurnoutSlipAxis);
    }
    else if (RaceVehicle->GetPhase() == EReverseRaceVehiclePhase::Racing)
    {
        RaceVehicle->SetThrottle(ThrottleAxis);
    }
}

void UReverseRaceInputComponent::SetBurnoutSlipAxis(float Value)
{
    BurnoutSlipAxis = FMath::Clamp(Value, 0.0f, 3.0f);
    if (RaceVehicle && RaceVehicle->GetPhase() == EReverseRaceVehiclePhase::Burnout)
    {
        RaceVehicle->SetBurnoutInput(ThrottleAxis, BurnoutSlipAxis);
    }
}

bool UReverseRaceInputComponent::SendCommand(EReverseRaceCommand Command, float ReactionTimeSeconds)
{
    if (!RaceVehicle)
    {
        return false;
    }

    switch (Command)
    {
        case EReverseRaceCommand::EnterBurnout:
            RaceVehicle->EnterBurnout();
            return true;
        case EReverseRaceCommand::FinishBurnout:
            RaceVehicle->FinishBurnout();
            return true;
        case EReverseRaceCommand::PreStage:
            RaceVehicle->PreStage();
            return true;
        case EReverseRaceCommand::Stage:
            RaceVehicle->Stage();
            return true;
        case EReverseRaceCommand::ArmTree:
            RaceVehicle->ArmTree();
            return true;
        case EReverseRaceCommand::Launch:
            return RaceVehicle->Launch(ReactionTimeSeconds, FMath::Max(0.01f, ThrottleAxis));
        case EReverseRaceCommand::ShiftUp:
            return RaceVehicle->ShiftUp();
        case EReverseRaceCommand::NitrousPressed:
            RaceVehicle->SetNitrous(true);
            return true;
        case EReverseRaceCommand::NitrousReleased:
            RaceVehicle->SetNitrous(false);
            return true;
        default:
            return false;
    }
}
