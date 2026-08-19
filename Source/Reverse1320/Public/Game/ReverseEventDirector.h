#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Game/ReverseProgressionTypes.h"
#include "ReverseEventDirector.generated.h"

UCLASS()
class REVERSE1320_API UReverseEventDirector : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category="Reverse1320|Events")
    static FReverseGeneratedMatch GenerateMatch(const FReverseRaceEventDefinition& Event,
                                                const FReverseOpponentProfile& Opponent,
                                                int32 Seed);

    UFUNCTION(BlueprintPure, Category="Reverse1320|Progression")
    static int32 ResolveLevel(int64 StreetCred, const TArray<FReverseCareerTier>& Tiers);
};