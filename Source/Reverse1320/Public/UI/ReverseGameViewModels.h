#pragma once

#include "CoreMinimal.h"
#include "Game/ReverseGameTypes.h"
#include "UI/ReverseGameViewModels.generated.h"

USTRUCT(BlueprintType)
struct FReverseGarageCardVM
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FName VehicleInstanceId;
    UPROPERTY(BlueprintReadOnly) FString DisplayName;
    UPROPERTY(BlueprintReadOnly) bool bActive = false;
    UPROPERTY(BlueprintReadOnly) int32 InstalledPartCount = 0;
    UPROPERTY(BlueprintReadOnly) FName ActiveTuneId;
};

USTRUCT(BlueprintType)
struct FReverseHeaderVM
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FString RacerName;
    UPROPERTY(BlueprintReadOnly) int32 Level = 1;
    UPROPERTY(BlueprintReadOnly) int64 Cash = 0;
    UPROPERTY(BlueprintReadOnly) int64 StreetCred = 0;
};

USTRUCT(BlueprintType)
struct FReverseEventCardVM
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FName EventId;
    UPROPERTY(BlueprintReadOnly) FText DisplayName;
    UPROPERTY(BlueprintReadOnly) bool bEligible = false;
    UPROPERTY(BlueprintReadOnly) FString LockReason;
    UPROPERTY(BlueprintReadOnly) int64 EntryFee = 0;
    UPROPERTY(BlueprintReadOnly) int64 WinPayout = 0;
};