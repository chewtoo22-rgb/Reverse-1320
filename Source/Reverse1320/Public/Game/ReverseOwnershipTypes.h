#pragma once

#include "CoreMinimal.h"
#include "Game/ReverseGameTypes.h"
#include "ReverseOwnershipTypes.generated.h"

USTRUCT(BlueprintType)
struct FReverseVehicleTransferBundle
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FReverseOwnedVehicle Vehicle;
    UPROPERTY(BlueprintReadOnly) TArray<FReverseOwnedPart> InstalledParts;
    UPROPERTY(BlueprintReadOnly) TArray<FReverseTunePreset> RelatedTunes;

    bool IsValid() const { return !Vehicle.InstanceId.IsNone() && !Vehicle.VehicleDefinitionId.IsNone(); }
};