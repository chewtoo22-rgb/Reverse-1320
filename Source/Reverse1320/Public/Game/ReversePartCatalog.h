#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Game/ReverseGameTypes.h"
#include "ReversePartCatalog.generated.h"

USTRUCT(BlueprintType)
struct FReversePartStatModifiers
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float TorqueMultiplier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float MassDeltaKg = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float GripMultiplier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float ShiftTimeMultiplier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float PeakBoostDeltaPsi = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float NitrousTorqueMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct FReversePartDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName PartId;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EReversePartSlot Slot = EReversePartSlot::EngineInternal;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 PurchasePrice = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FName> AllowedVehicleTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FName> BlockedVehicleTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FName> RequiredInstalledPartIds;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FName> ConflictingInstalledPartIds;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FReversePartStatModifiers Modifiers;
};

UCLASS(BlueprintType)
class REVERSE1320_API UReversePartCatalog : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FReversePartDefinition> Parts;

    UFUNCTION(BlueprintPure, Category="Reverse1320|Parts")
    bool FindPart(FName PartId, FReversePartDefinition& OutPart) const;

    UFUNCTION(BlueprintPure, Category="Reverse1320|Parts")
    bool IsCompatible(const FReversePartDefinition& Part, const TArray<FName>& VehicleTags,
                      const TArray<FName>& InstalledPartIds, FString& FailureReason) const;
};