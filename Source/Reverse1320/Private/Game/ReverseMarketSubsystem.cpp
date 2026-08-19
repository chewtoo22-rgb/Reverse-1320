#include "Game/ReverseMarketSubsystem.h"
#include "Game/ReverseCareerSubsystem.h"
#include "Engine/GameInstance.h"

UReverseCareerSubsystem* UReverseMarketSubsystem::Career() const
{
    UGameInstance* GI = GetGameInstance();
    return GI ? GI->GetSubsystem<UReverseCareerSubsystem>() : nullptr;
}

FReverseMarketResult UReverseMarketSubsystem::PurchaseVehicle(FName ListingId)
{
    FReverseMarketResult Result;
    UReverseCareerSubsystem* C = Career();
    if (!C) { Result.Message = TEXT("Career subsystem unavailable"); return Result; }
    const FReverseVehicleListing* L = VehicleListings.FindByPredicate([ListingId](const FReverseVehicleListing& X){ return X.ListingId == ListingId; });
    if (!L) { Result.Message = TEXT("Vehicle listing not found"); return Result; }
    const FReversePlayerProfile& P = C->GetProfile();
    if (P.Level < L->MinimumLevel || P.StreetCred < L->MinimumStreetCred) { Result.Message = TEXT("Vehicle is still locked"); return Result; }
    if (!C->ApplyTransaction(EReverseTransactionType::Purchase, -L->Price, FString::Printf(TEXT("Vehicle purchase: %s"), *L->VehicleDefinitionId.ToString()))) { Result.Message = TEXT("Not enough cash"); return Result; }
    const FName Instance(*FString::Printf(TEXT("veh_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits)));
    if (!C->AddVehicle(L->VehicleDefinitionId, Instance, L->DisplayName.ToString()))
    {
        C->ApplyTransaction(EReverseTransactionType::Sale, L->Price, TEXT("Vehicle purchase rollback"));
        Result.Message = TEXT("Unable to add vehicle"); return Result;
    }
    Result.bSuccess = true; Result.Message = TEXT("Vehicle purchased"); Result.CreatedInstanceId = Instance;
    return Result;
}

FReverseMarketResult UReverseMarketSubsystem::PurchasePart(FName ListingId)
{
    FReverseMarketResult Result;
    UReverseCareerSubsystem* C = Career();
    if (!C) { Result.Message = TEXT("Career subsystem unavailable"); return Result; }
    const FReversePartListing* L = PartListings.FindByPredicate([ListingId](const FReversePartListing& X){ return X.ListingId == ListingId; });
    if (!L) { Result.Message = TEXT("Part listing not found"); return Result; }
    const FReversePlayerProfile& P = C->GetProfile();
    if (P.Level < L->MinimumLevel || P.StreetCred < L->MinimumStreetCred) { Result.Message = TEXT("Part is still locked"); return Result; }
    if (!C->ApplyTransaction(EReverseTransactionType::Purchase, -L->Price, FString::Printf(TEXT("Part purchase: %s"), *L->PartId.ToString()))) { Result.Message = TEXT("Not enough cash"); return Result; }
    const FName Instance(*FString::Printf(TEXT("part_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits)));
    if (!C->AddPart(L->PartId, Instance, L->Slot))
    {
        C->ApplyTransaction(EReverseTransactionType::Sale, L->Price, TEXT("Part purchase rollback"));
        Result.Message = TEXT("Unable to add part"); return Result;
    }
    Result.bSuccess = true; Result.Message = TEXT("Part purchased"); Result.CreatedInstanceId = Instance;
    return Result;
}