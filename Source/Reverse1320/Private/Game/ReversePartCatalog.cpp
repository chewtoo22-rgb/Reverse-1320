#include "Game/ReversePartCatalog.h"

bool UReversePartCatalog::FindPart(FName PartId, FReversePartDefinition& OutPart) const
{
    if (const FReversePartDefinition* Match = Parts.FindByPredicate([PartId](const FReversePartDefinition& P){ return P.PartId == PartId; }))
    {
        OutPart = *Match;
        return true;
    }
    return false;
}

bool UReversePartCatalog::IsCompatible(const FReversePartDefinition& Part, const TArray<FName>& VehicleTags,
                                       const TArray<FName>& InstalledPartIds, FString& FailureReason) const
{
    FailureReason.Empty();

    if (Part.AllowedVehicleTags.Num() > 0)
    {
        const bool bAllowed = Part.AllowedVehicleTags.ContainsByPredicate([&VehicleTags](FName Tag){ return VehicleTags.Contains(Tag); });
        if (!bAllowed) { FailureReason = TEXT("Vehicle does not match this part's compatibility tags"); return false; }
    }

    if (Part.BlockedVehicleTags.ContainsByPredicate([&VehicleTags](FName Tag){ return VehicleTags.Contains(Tag); }))
    {
        FailureReason = TEXT("Vehicle has a blocked compatibility tag");
        return false;
    }

    for (FName Required : Part.RequiredInstalledPartIds)
    {
        if (!InstalledPartIds.Contains(Required))
        {
            FailureReason = FString::Printf(TEXT("Required part missing: %s"), *Required.ToString());
            return false;
        }
    }

    for (FName Conflict : Part.ConflictingInstalledPartIds)
    {
        if (InstalledPartIds.Contains(Conflict))
        {
            FailureReason = FString::Printf(TEXT("Conflicting part installed: %s"), *Conflict.ToString());
            return false;
        }
    }
    return true;
}