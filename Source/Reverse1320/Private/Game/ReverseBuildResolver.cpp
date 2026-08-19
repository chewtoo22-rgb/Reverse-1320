#include "Game/ReverseBuildResolver.h"

FReverseResolvedBuildStats UReverseBuildResolver::Resolve(const UReverseVehicleDefinition* Vehicle,
                                                          const TArray<FReversePartDefinition>& InstalledParts)
{
    FReverseResolvedBuildStats Out;
    if (!Vehicle) return Out;

    Out.MassKg = Vehicle->MassKg;
    Out.ShiftTimeSeconds = Vehicle->Gearbox.ShiftTimeSeconds;
    Out.PeakBoostPsi = Vehicle->PeakBoostPsi;
    Out.NitrousTorqueMultiplier = Vehicle->NitrousTorqueMultiplier;

    for (const FReversePartDefinition& Part : InstalledParts)
    {
        Out.MassKg += Part.Modifiers.MassDeltaKg;
        Out.TorqueMultiplier *= FMath::Max(0.01f, Part.Modifiers.TorqueMultiplier);
        Out.GripMultiplier *= FMath::Max(0.01f, Part.Modifiers.GripMultiplier);
        Out.ShiftTimeSeconds *= FMath::Max(0.05f, Part.Modifiers.ShiftTimeMultiplier);
        Out.PeakBoostPsi += Part.Modifiers.PeakBoostDeltaPsi;
        Out.NitrousTorqueMultiplier *= FMath::Max(1.0f, Part.Modifiers.NitrousTorqueMultiplier);
    }

    Out.MassKg = FMath::Max(250.0f, Out.MassKg);
    Out.ShiftTimeSeconds = FMath::Clamp(Out.ShiftTimeSeconds, 0.02f, 2.0f);
    Out.PeakBoostPsi = FMath::Max(0.0f, Out.PeakBoostPsi);
    return Out;
}