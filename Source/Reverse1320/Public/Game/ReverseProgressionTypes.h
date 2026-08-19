#pragma once

#include "CoreMinimal.h"
#include "Game/ReverseGameTypes.h"
#include "ReverseProgressionTypes.generated.h"

USTRUCT(BlueprintType)
struct FReverseCareerTier
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Level = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 StreetCredRequired = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 CashReward = 0;
};

USTRUCT(BlueprintType)
struct FReverseOpponentProfile
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName OpponentId;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName VehicleClassTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float TargetQuarterET = 13.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float ReactionMean = 0.180f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float ReactionJitter = 0.060f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float ConsistencySeconds = 0.120f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 1;
};

USTRUCT(BlueprintType)
struct FReverseGeneratedMatch
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FName EventId;
    UPROPERTY(BlueprintReadOnly) FName OpponentId;
    UPROPERTY(BlueprintReadOnly) float OpponentET = 0.0f;
    UPROPERTY(BlueprintReadOnly) float OpponentReaction = 0.0f;
    UPROPERTY(BlueprintReadOnly) int64 EntryFee = 0;
    UPROPERTY(BlueprintReadOnly) int64 WinPayout = 0;
};