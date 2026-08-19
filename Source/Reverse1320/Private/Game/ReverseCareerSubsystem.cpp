#include "Game/ReverseCareerSubsystem.h"
#include "Game/ReverseProfileSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UReverseCareerSubsystem::CreateNewProfile(const FString& RacerName, int64 StartingCash)
{
    Profile = FReversePlayerProfile();
    Profile.SchemaVersion = UReverseProfileSaveGame::CurrentSchemaVersion;
    Profile.RacerName = RacerName;
    Profile.Cash = 0;
    ApplyTransaction(EReverseTransactionType::StartingFunds, FMath::Max<int64>(0, StartingCash), TEXT("New racer starting funds"));
}

bool UReverseCareerSubsystem::SaveProfile(const FString& SlotName, int32 UserIndex)
{
    UReverseProfileSaveGame* Save = Cast<UReverseProfileSaveGame>(UGameplayStatics::CreateSaveGameObject(UReverseProfileSaveGame::StaticClass()));
    if (!Save) return false;
    Profile.SchemaVersion = UReverseProfileSaveGame::CurrentSchemaVersion;
    Save->SchemaVersion = UReverseProfileSaveGame::CurrentSchemaVersion;
    Save->Profile = Profile;
    Save->SavedAtUtc = FDateTime::UtcNow();
    return UGameplayStatics::SaveGameToSlot(Save, SlotName, UserIndex);
}

bool UReverseCareerSubsystem::LoadProfile(const FString& SlotName, int32 UserIndex)
{
    UReverseProfileSaveGame* Save = Cast<UReverseProfileSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    if (!Save) return false;
    FReversePlayerProfile Loaded = Save->Profile;
    if (!MigrateProfileIfNeeded(Loaded, Save->SchemaVersion)) return false;
    Profile = MoveTemp(Loaded);
    return true;
}

bool UReverseCareerSubsystem::MigrateProfileIfNeeded(FReversePlayerProfile& InOutProfile, int32 SavedSchemaVersion) const
{
    if (SavedSchemaVersion > UReverseProfileSaveGame::CurrentSchemaVersion) return false;
    InOutProfile.SchemaVersion = UReverseProfileSaveGame::CurrentSchemaVersion;
    return true;
}

FReverseOwnedVehicle* UReverseCareerSubsystem::FindVehicle(FName InstanceId)
{
    return Profile.Garage.FindByPredicate([InstanceId](const FReverseOwnedVehicle& V){ return V.InstanceId == InstanceId; });
}

FReverseOwnedPart* UReverseCareerSubsystem::FindPart(FName InstanceId)
{
    return Profile.Inventory.FindByPredicate([InstanceId](const FReverseOwnedPart& P){ return P.InstanceId == InstanceId; });
}

bool UReverseCareerSubsystem::AddVehicle(FName VehicleDefinitionId, FName InstanceId, const FString& Nickname)
{
    if (VehicleDefinitionId.IsNone() || InstanceId.IsNone() || FindVehicle(InstanceId)) return false;
    FReverseOwnedVehicle V;
    V.VehicleDefinitionId = VehicleDefinitionId;
    V.InstanceId = InstanceId;
    V.Nickname = Nickname;
    Profile.Garage.Add(V);
    if (Profile.ActiveVehicleInstance.IsNone()) Profile.ActiveVehicleInstance = InstanceId;
    return true;
}

bool UReverseCareerSubsystem::SetActiveVehicle(FName InstanceId)
{
    if (!FindVehicle(InstanceId)) return false;
    Profile.ActiveVehicleInstance = InstanceId;
    return true;
}

bool UReverseCareerSubsystem::ExtractVehicleForTransfer(FName VehicleInstanceId, FReverseVehicleTransferBundle& OutBundle)
{
    OutBundle = FReverseVehicleTransferBundle();
    const int32 VehicleIndex = Profile.Garage.IndexOfByPredicate([VehicleInstanceId](const FReverseOwnedVehicle& V){ return V.InstanceId == VehicleInstanceId; });
    if (VehicleIndex == INDEX_NONE) return false;

    OutBundle.Vehicle = Profile.Garage[VehicleIndex];
    for (const FName PartInstanceId : OutBundle.Vehicle.InstalledPartInstances)
    {
        const int32 PartIndex = Profile.Inventory.IndexOfByPredicate([PartInstanceId](const FReverseOwnedPart& P){ return P.InstanceId == PartInstanceId; });
        if (PartIndex != INDEX_NONE)
        {
            OutBundle.InstalledParts.Add(Profile.Inventory[PartIndex]);
        }
    }
    if (!OutBundle.Vehicle.ActiveTuneId.IsNone())
    {
        if (const FReverseTunePreset* Tune = Profile.Tunes.FindByPredicate([&](const FReverseTunePreset& T){ return T.TuneId == OutBundle.Vehicle.ActiveTuneId; }))
            OutBundle.RelatedTunes.Add(*Tune);
    }

    if (!OutBundle.IsValid()) return false;

    for (const FReverseOwnedPart& Part : OutBundle.InstalledParts)
        Profile.Inventory.RemoveAll([&Part](const FReverseOwnedPart& P){ return P.InstanceId == Part.InstanceId; });
    for (const FReverseTunePreset& Tune : OutBundle.RelatedTunes)
        Profile.Tunes.RemoveAll([&Tune](const FReverseTunePreset& T){ return T.TuneId == Tune.TuneId; });
    Profile.Garage.RemoveAt(VehicleIndex);

    if (Profile.ActiveVehicleInstance == VehicleInstanceId)
        Profile.ActiveVehicleInstance = Profile.Garage.Num() > 0 ? Profile.Garage[0].InstanceId : NAME_None;
    return true;
}

bool UReverseCareerSubsystem::ImportTransferredVehicle(const FReverseVehicleTransferBundle& Bundle)
{
    if (!Bundle.IsValid() || FindVehicle(Bundle.Vehicle.InstanceId)) return false;
    for (const FReverseOwnedPart& Incoming : Bundle.InstalledParts)
        if (FindPart(Incoming.InstanceId)) return false;

    Profile.Garage.Add(Bundle.Vehicle);
    for (const FReverseOwnedPart& Part : Bundle.InstalledParts) Profile.Inventory.Add(Part);
    for (const FReverseTunePreset& Tune : Bundle.RelatedTunes)
    {
        if (!Profile.Tunes.ContainsByPredicate([&](const FReverseTunePreset& T){ return T.TuneId == Tune.TuneId; })) Profile.Tunes.Add(Tune);
    }
    if (Profile.ActiveVehicleInstance.IsNone()) Profile.ActiveVehicleInstance = Bundle.Vehicle.InstanceId;
    return true;
}

bool UReverseCareerSubsystem::AddPart(FName PartId, FName InstanceId, EReversePartSlot Slot)
{
    if (PartId.IsNone() || InstanceId.IsNone() || FindPart(InstanceId)) return false;
    FReverseOwnedPart P; P.PartId = PartId; P.InstanceId = InstanceId; P.Slot = Slot; Profile.Inventory.Add(P); return true;
}

bool UReverseCareerSubsystem::InstallPart(FName VehicleInstanceId, FName PartInstanceId)
{
    FReverseOwnedVehicle* Vehicle = FindVehicle(VehicleInstanceId);
    FReverseOwnedPart* Part = FindPart(PartInstanceId);
    if (!Vehicle || !Part) return false;
    for (int32 i = Vehicle->InstalledPartInstances.Num() - 1; i >= 0; --i)
    {
        FReverseOwnedPart* Existing = FindPart(Vehicle->InstalledPartInstances[i]);
        if (Existing && Existing->Slot == Part->Slot) { Existing->bInstalled = false; Vehicle->InstalledPartInstances.RemoveAt(i); }
    }
    if (!Vehicle->InstalledPartInstances.Contains(PartInstanceId)) Vehicle->InstalledPartInstances.Add(PartInstanceId);
    Part->bInstalled = true; return true;
}

bool UReverseCareerSubsystem::RemovePart(FName VehicleInstanceId, FName PartInstanceId)
{
    FReverseOwnedVehicle* Vehicle = FindVehicle(VehicleInstanceId); FReverseOwnedPart* Part = FindPart(PartInstanceId);
    if (!Vehicle || !Part || !Vehicle->InstalledPartInstances.Remove(PartInstanceId)) return false;
    Part->bInstalled = false; return true;
}

bool UReverseCareerSubsystem::SaveTune(FName VehicleInstanceId, const FReverseTunePreset& Tune)
{
    FReverseOwnedVehicle* Vehicle = FindVehicle(VehicleInstanceId);
    if (!Vehicle || Tune.TuneId.IsNone()) return false;
    if (FReverseTunePreset* Existing = Profile.Tunes.FindByPredicate([&Tune](const FReverseTunePreset& T){ return T.TuneId == Tune.TuneId; })) *Existing = Tune; else Profile.Tunes.Add(Tune);
    Vehicle->ActiveTuneId = Tune.TuneId; return true;
}

bool UReverseCareerSubsystem::ApplyTransaction(EReverseTransactionType Type, int64 Amount, const FString& Reason)
{
    if (Amount < 0 && Profile.Cash < -Amount) return false;
    Profile.Cash += Amount;
    FReverseLedgerEntry Entry; Entry.TransactionId = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower); Entry.Type = Type; Entry.Amount = Amount; Entry.Reason = Reason; Entry.TimestampUtc = FDateTime::UtcNow();
    Profile.Ledger.Add(Entry); return true;
}

bool UReverseCareerSubsystem::IsEligibleForEvent(const FReverseRaceEventDefinition& Event, FString& FailureReason) const
{
    FailureReason.Empty();
    if (Profile.Level < Event.MinimumLevel) { FailureReason = TEXT("Racer level too low"); return false; }
    if (Profile.ActiveVehicleInstance.IsNone()) { FailureReason = TEXT("No active vehicle"); return false; }
    if (Profile.Cash < Event.EntryFee) { FailureReason = TEXT("Not enough cash for entry fee"); return false; }
    return true;
}

bool UReverseCareerSubsystem::EnterEvent(const FReverseRaceEventDefinition& Event, FString& FailureReason)
{
    if (!IsEligibleForEvent(Event, FailureReason)) return false;
    return Event.EntryFee <= 0 || ApplyTransaction(EReverseTransactionType::RaceEntry, -Event.EntryFee, FString::Printf(TEXT("Entry: %s"), *Event.EventId.ToString()));
}

void UReverseCareerSubsystem::SettleEvent(const FReverseRaceEventDefinition& Event, bool bWon)
{
    if (!bWon) return;
    if (Event.WinPayout > 0) ApplyTransaction(EReverseTransactionType::RaceWin, Event.WinPayout, FString::Printf(TEXT("Win: %s"), *Event.EventId.ToString()));
    Profile.StreetCred += FMath::Max<int64>(0, Event.StreetCredReward);
}