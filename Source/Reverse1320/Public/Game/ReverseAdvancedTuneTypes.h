#pragma once

#include "CoreMinimal.h"
#include "ReverseAdvancedTuneTypes.generated.h"

USTRUCT(BlueprintType)
struct FReverseAdvancedTune
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) float LaunchRPM = 3200.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ShiftRPM = 6800.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float TirePressurePsi = 18.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float FinalDrive = 3.55f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BoostTargetPsi = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BoostRampSeconds = 0.50f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float NitrousDelaySeconds = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float NitrousRampSeconds = 0.20f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float FrontDamping = 0.50f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float RearDamping = 0.50f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float WheelieBarHeightMeters = 0.08f;
};

USTRUCT(BlueprintType)
struct FReverseTuneValidation
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) bool bValid = true;
    UPROPERTY(BlueprintReadOnly) TArray<FString> Warnings;
};

struct FReverseTuneRules
{
    static FReverseTuneValidation Validate(const FReverseAdvancedTune& Tune, float RedlineRPM)
    {
        FReverseTuneValidation Out;
        if (Tune.LaunchRPM < 800.0f || Tune.LaunchRPM >= RedlineRPM) Out.Warnings.Add(TEXT("Launch RPM is outside the usable engine range"));
        if (Tune.ShiftRPM <= Tune.LaunchRPM || Tune.ShiftRPM > RedlineRPM * 1.05f) Out.Warnings.Add(TEXT("Shift RPM is inconsistent with launch/redline"));
        if (Tune.TirePressurePsi < 6.0f || Tune.TirePressurePsi > 45.0f) Out.Warnings.Add(TEXT("Tire pressure is outside supported race range"));
        if (Tune.FinalDrive < 2.0f || Tune.FinalDrive > 6.5f) Out.Warnings.Add(TEXT("Final drive is outside supported range"));
        if (Tune.BoostTargetPsi < 0.0f || Tune.BoostTargetPsi > 80.0f) Out.Warnings.Add(TEXT("Boost target is outside supported range"));
        if (Tune.NitrousDelaySeconds < 0.0f || Tune.NitrousDelaySeconds > 10.0f) Out.Warnings.Add(TEXT("Nitrous delay is outside supported range"));
        if (Tune.WheelieBarHeightMeters < 0.0f || Tune.WheelieBarHeightMeters > 0.30f) Out.Warnings.Add(TEXT("Wheelie-bar height is outside supported range"));
        Out.bValid = Out.Warnings.Num() == 0;
        return Out;
    }
};