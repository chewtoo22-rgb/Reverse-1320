#include "Game/ReverseCareerSubsystem.h"

void UReverseCareerSubsystem::CreateNewProfile(const FString& RacerName, int64 StartingCash)
{
    Profile = FReversePlayerProfile();
    Profile.RacerName = RacerName;
    Profile.Cash = 0;
    ApplyTransaction(EReverseTransactionType::StartingFunds, FMath::Max<int64>(0, StartingCash), TEXT("New racer starting funds"));
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

bool UReverseCareerSubsystem::AddPart(FName PartId, FName InstanceId, EReversePartSlot Slot)
{
    if (PartId.IsNone() || InstanceId.IsNone() || FindPart(InstanceId)) return false;
    FReverseOwnedPart P;
    P.PartId = PartId; P.InstanceId = InstanceId; P.Slot = Slot;
    Profile.Inventory.Add(P);
    return true;
}

bool UReverseCareerSubsystem::InstallPart(FName VehicleInstanceId, FName PartInstanceId)
{
    FReverseOwnedVehicle* Vehicle = FindVehicle(VehicleInstanceId);
    FReverseOwnedPart* Part = FindPart(PartInstanceId);
    if (!Vehicle || !Part) return false;

    // One installed part per slot for the first clean implementation. Replaced parts return to inventory.
    for (int32 i = Vehicle->InstalledPartInstances.Num() - 1; i >= 0; --i)
    {
        FReverseOwnedPart* Existing = FindPart(Vehicle->InstalledPartInstances[i]);
        if (Existing && Existing->Slot == Part->Slot)
        {
            Existing->bInstalled = false;
            Vehicle->InstalledPartInstances.RemoveAt(i);
        }
    }
    if (!Vehicle->InstalledPartInstances.Contains(PartInstanceId)) Vehicle->InstalledPartInstances.Add(PartInstanceId);
    Part->bInstalled = true;
    return true;
}

bool UReverseCareerSubsystem::RemovePart(FName VehicleInstanceId, FName PartInstanceId)
{
    FReverseOwnedVehicle* Vehicle = FindVehicle(VehicleInstanceId);
    FReverseOwnedPart* Part = FindPart(PartInstanceId);
    if (!Vehicle || !Part || !Vehicle->InstalledPartInstances.Remove(PartInstanceId)) return false;
    Part->bInstalled = false;
    return true;
}

bool UReverseCareerSubsystem::SaveTune(FName VehicleInstanceId, const FReverseTunePreset& Tune)
{
    FReverseOwnedVehicle* Vehicle = FindVehicle(VehicleInstanceId);
    if (!Vehicle || Tune.TuneId.IsNone()) return false;
    if (FReverseTunePreset* Existing = Profile.Tunes.FindByPredicate([&Tune](const FReverseTunePreset& T){ return T.TuneId == Tune.TuneId; })) *Existing = Tune;
    else Profile.Tunes.Add(Tune);
    Vehicle->ActiveTuneId = Tune.TuneId;
    return true;
}

bool UReverseCareerSubsystem::ApplyTransaction(EReverseTransactionType Type, int64 Amount, const FString& Reason)
{
    if (Amount < 0 && Profile.Cash < -Amount) return false;
    Profile.Cash += Amount;
    FReverseLedgerEntry Entry;
    Entry.TransactionId = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
    Entry.Type = Type; Entry.Amount = Amount; Entry.Reason = Reason; Entry.TimestampUtc = FDateTime::UtcNow();
    Profile.Ledger.Add(Entry);
    return true;
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