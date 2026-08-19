#pragma once

#include "CoreMinimal.h"
#include "ReverseGameTypes.generated.h"

UENUM(BlueprintType)
enum class EReversePartSlot : uint8
{
    EngineInternal, Intake, ForcedInduction, Fuel, Exhaust, Nitrous,
    Transmission, Differential, Tire, Suspension, WeightReduction, Electronics
};

UENUM(BlueprintType)
enum class EReverseTransactionType : uint8
{
    StartingFunds, Purchase, Sale, RaceEntry, RaceWin, RaceLoss, SponsorReward, ProgressionReward
};

USTRUCT(BlueprintType)
struct FReverseOwnedPart
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName InstanceId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PartId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EReversePartSlot Slot = EReversePartSlot::EngineInternal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bInstalled = false;
};

USTRUCT(BlueprintType)
struct FReverseOwnedVehicle
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName InstanceId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName VehicleDefinitionId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Nickname;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> InstalledPartInstances;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ActiveTuneId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 OdometerRaceMiles = 0;
};

USTRUCT(BlueprintType)
struct FReverseTunePreset
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TuneId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float LaunchRPM = 3200.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ShiftRPM = 6800.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float TirePressurePsi = 18.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float NitrousActivationSeconds = 0.0f;
};

USTRUCT(BlueprintType)
struct FReverseLedgerEntry
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TransactionId;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EReverseTransactionType Type = EReverseTransactionType::StartingFunds;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 Amount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Reason;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FDateTime TimestampUtc;
};

USTRUCT(BlueprintType)
struct FReversePlayerProfile
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 SchemaVersion = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString RacerName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 StreetCred = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 Cash = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ActiveVehicleInstance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FReverseOwnedVehicle> Garage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FReverseOwnedPart> Inventory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FReverseTunePreset> Tunes;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FReverseLedgerEntry> Ledger;
};

USTRUCT(BlueprintType)
struct FReverseRaceEventDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EventId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MinimumLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 EntryFee = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 WinPayout = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 StreetCredReward = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bPinksAllowed = false;
};