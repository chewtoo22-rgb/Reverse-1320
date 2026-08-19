#pragma once

#include "CoreMinimal.h"
#include "ReverseOpponentTypes.generated.h"

UENUM(BlueprintType)
enum class EReverseRacerTemperament : uint8
{
    Conservative,
    Calculated,
    Aggressive,
    Wildcard
};

USTRUCT(BlueprintType)
struct FReverseRacerPersonality
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PersonalityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EReverseRacerTemperament Temperament = EReverseRacerTemperament::Calculated;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReactionMeanSeconds = 0.22f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReactionVarianceSeconds = 0.05f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ShiftConsistency = 0.85f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float NitrousAggression = 0.50f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float RiskTolerance = 0.50f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float PinksAcceptance = 0.10f;
};

USTRUCT(BlueprintType)
struct FReverseOpponentPlan
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) float ReactionTimeSeconds = 0.22f;
    UPROPERTY(BlueprintReadOnly) float ShiftErrorSeconds = 0.0f;
    UPROPERTY(BlueprintReadOnly) float NitrousDelaySeconds = 0.0f;
    UPROPERTY(BlueprintReadOnly) bool bAcceptPinks = false;
};

struct FReverseOpponentPlanner
{
    static FReverseOpponentPlan BuildPlan(const FReverseRacerPersonality& Personality, int32 Seed)
    {
        FRandomStream Rng(Seed);
        FReverseOpponentPlan Out;
        const float Variance = FMath::Max(0.0f, Personality.ReactionVarianceSeconds);
        Out.ReactionTimeSeconds = FMath::Clamp(Personality.ReactionMeanSeconds + Rng.FRandRange(-Variance, Variance), -0.20f, 0.75f);
        Out.ShiftErrorSeconds = Rng.FRandRange(0.0f, 0.12f) * (1.0f - FMath::Clamp(Personality.ShiftConsistency, 0.0f, 1.0f));
        Out.NitrousDelaySeconds = FMath::Lerp(1.25f, 0.05f, FMath::Clamp(Personality.NitrousAggression, 0.0f, 1.0f)) + Rng.FRandRange(0.0f, 0.20f);
        Out.bAcceptPinks = Rng.FRand() < FMath::Clamp(Personality.PinksAcceptance * (0.5f + Personality.RiskTolerance), 0.0f, 1.0f);
        return Out;
    }
};