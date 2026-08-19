#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Game/ReverseMarketTypes.h"
#include "ReverseMarketSubsystem.generated.h"

class UReverseCareerSubsystem;

UCLASS()
class REVERSE1320_API UReverseMarketSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Reverse1320|Market")
    void SetVehicleListings(const TArray<FReverseVehicleListing>& Listings) { VehicleListings = Listings; }

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Market")
    void SetPartListings(const TArray<FReversePartListing>& Listings) { PartListings = Listings; }

    UFUNCTION(BlueprintPure, Category="Reverse1320|Market")
    const TArray<FReverseVehicleListing>& GetVehicleListings() const { return VehicleListings; }

    UFUNCTION(BlueprintPure, Category="Reverse1320|Market")
    const TArray<FReversePartListing>& GetPartListings() const { return PartListings; }

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Market")
    FReverseMarketResult PurchaseVehicle(FName ListingId);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Market")
    FReverseMarketResult PurchasePart(FName ListingId);

private:
    UReverseCareerSubsystem* Career() const;
    TArray<FReverseVehicleListing> VehicleListings;
    TArray<FReversePartListing> PartListings;
};