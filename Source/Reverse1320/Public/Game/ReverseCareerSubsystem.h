#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Game/ReverseGameTypes.h"
#include "Game/ReverseOwnershipTypes.h"
#include "ReverseCareerSubsystem.generated.h"

UCLASS()
class REVERSE1320_API UReverseCareerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Reverse1320|Career")
    void CreateNewProfile(const FString& RacerName, int64 StartingCash = 15000);

    UFUNCTION(BlueprintPure, Category="Reverse1320|Career")
    const FReversePlayerProfile& GetProfile() const { return Profile; }

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Career")
    bool SaveProfile(const FString& SlotName = TEXT("Reverse1320_Profile"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Career")
    bool LoadProfile(const FString& SlotName = TEXT("Reverse1320_Profile"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Garage")
    bool AddVehicle(FName VehicleDefinitionId, FName InstanceId, const FString& Nickname);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Garage")
    bool SetActiveVehicle(FName InstanceId);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Garage")
    bool ExtractVehicleForTransfer(FName VehicleInstanceId, FReverseVehicleTransferBundle& OutBundle);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Garage")
    bool ImportTransferredVehicle(const FReverseVehicleTransferBundle& Bundle);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Inventory")
    bool AddPart(FName PartId, FName InstanceId, EReversePartSlot Slot);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Inventory")
    bool InstallPart(FName VehicleInstanceId, FName PartInstanceId);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Inventory")
    bool RemovePart(FName VehicleInstanceId, FName PartInstanceId);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Tuning")
    bool SaveTune(FName VehicleInstanceId, const FReverseTunePreset& Tune);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Economy")
    bool ApplyTransaction(EReverseTransactionType Type, int64 Amount, const FString& Reason);

    UFUNCTION(BlueprintPure, Category="Reverse1320|Events")
    bool IsEligibleForEvent(const FReverseRaceEventDefinition& Event, FString& FailureReason) const;

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Events")
    bool EnterEvent(const FReverseRaceEventDefinition& Event, FString& FailureReason);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Events")
    void SettleEvent(const FReverseRaceEventDefinition& Event, bool bWon);

private:
    bool MigrateProfileIfNeeded(FReversePlayerProfile& InOutProfile, int32 SavedSchemaVersion) const;
    FReverseOwnedVehicle* FindVehicle(FName InstanceId);
    FReverseOwnedPart* FindPart(FName InstanceId);
    FReversePlayerProfile Profile;
};