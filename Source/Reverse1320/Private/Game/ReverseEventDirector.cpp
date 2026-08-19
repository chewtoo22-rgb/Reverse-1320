#include "Game/ReverseEventDirector.h"

FReverseGeneratedMatch UReverseEventDirector::GenerateMatch(const FReverseRaceEventDefinition& Event,
                                                            const FReverseOpponentProfile& Opponent,
                                                            int32 Seed)
{
    FRandomStream Rng(Seed);
    FReverseGeneratedMatch Out;
    Out.EventId = Event.EventId;
    Out.OpponentId = Opponent.OpponentId;
    Out.OpponentET = FMath::Max(4.0f, Opponent.TargetQuarterET + Rng.FRandRange(-Opponent.ConsistencySeconds, Opponent.ConsistencySeconds));
    Out.OpponentReaction = FMath::Max(0.0f, Opponent.ReactionMean + Rng.FRandRange(-Opponent.ReactionJitter, Opponent.ReactionJitter));
    Out.EntryFee = Event.EntryFee;
    Out.WinPayout = Event.WinPayout;
    return Out;
}

int32 UReverseEventDirector::ResolveLevel(int64 StreetCred, const TArray<FReverseCareerTier>& Tiers)
{
    int32 Resolved = 1;
    for (const FReverseCareerTier& Tier : Tiers)
    {
        if (StreetCred >= Tier.StreetCredRequired)
        {
            Resolved = FMath::Max(Resolved, Tier.Level);
        }
    }
    return Resolved;
}