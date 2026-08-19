#pragma once

#include "CoreMinimal.h"
#include "ReverseRaceSessionTypes.generated.h"

UENUM(BlueprintType)
enum class EReverseRaceStakeType : uint8
{
    Friendly,
    Cash,
    Pinks
};

UENUM(BlueprintType)
enum class EReverseRaceSessionState : uint8
{
    Lobby,
    Locked,
    Staging,
    Racing,
    Finished,
    Settled,
    Cancelled
};

USTRUCT(BlueprintType)
struct FReverseRaceParticipant
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString RacerId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName VehicleInstanceId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bReady = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReactionTime = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ElapsedTime = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float TrapMph = 0.0f;
};

USTRUCT(BlueprintType)
struct FReverseRaceSession
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SessionId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EReverseRaceSessionState State = EReverseRaceSessionState::Lobby;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EReverseRaceStakeType StakeType = EReverseRaceStakeType::Friendly;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 CashStakePerRacer = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FReverseRaceParticipant> Participants;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString WinnerRacerId;
};

USTRUCT(BlueprintType)
struct FReverseRaceSettlement
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) bool bSuccess = false;
    UPROPERTY(BlueprintReadOnly) FString Message;
    UPROPERTY(BlueprintReadOnly) int64 CashDelta = 0;
    UPROPERTY(BlueprintReadOnly) FName VehicleWon;
    UPROPERTY(BlueprintReadOnly) FName VehicleLost;
};