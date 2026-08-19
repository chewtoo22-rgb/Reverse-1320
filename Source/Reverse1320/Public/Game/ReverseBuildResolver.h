#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Vehicle/ReverseVehicleTypes.h"
#include "Game/ReversePartCatalog.h"
#include "ReverseBuildResolver.generated.h"

USTRUCT(BlueprintType)
struct FReverseResolvedBuildStats
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) float MassKg = 0.0f;
    UPROPERTY(BlueprintReadOnly) float TorqueMultiplier = 1.0f;
    UPROPERTY(BlueprintReadOnly) float GripMultiplier = 1.0f;
    UPROPERTY(BlueprintReadOnly) float ShiftTimeSeconds = 0.0f;
    UPROPERTY(BlueprintReadOnly) float PeakBoostPsi = 0.0f;
    UPROPERTY(BlueprintReadOnly) float NitrousTorqueMultiplier = 1.0f;
};

UCLASS()
class REVERSE1320_API UReverseBuildResolver : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category="Reverse1320|Build")
    static FReverseResolvedBuildStats Resolve(const UReverseVehicleDefinition* Vehicle,
                                              const TArray<FReversePartDefinition>& InstalledParts);
};