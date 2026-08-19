#pragma once

#include "CoreMinimal.h"
#include "Game/ReverseGameTypes.h"
#include "ReverseMarketTypes.generated.h"

USTRUCT(BlueprintType)
struct FReverseVehicleListing
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ListingId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName VehicleDefinitionId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 Price = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MinimumLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 MinimumStreetCred = 0;
};

USTRUCT(BlueprintType)
struct FReversePartListing
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ListingId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PartId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EReversePartSlot Slot = EReversePartSlot::EngineInternal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 Price = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MinimumLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 MinimumStreetCred = 0;
};

USTRUCT(BlueprintType)
struct FReverseMarketResult
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) bool bSuccess = false;
    UPROPERTY(BlueprintReadOnly) FString Message;
    UPROPERTY(BlueprintReadOnly) FName CreatedInstanceId;
};