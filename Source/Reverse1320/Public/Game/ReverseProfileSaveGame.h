#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Game/ReverseGameTypes.h"
#include "ReverseProfileSaveGame.generated.h"

UCLASS()
class REVERSE1320_API UReverseProfileSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    static constexpr int32 CurrentSchemaVersion = 1;

    UPROPERTY(BlueprintReadWrite) int32 SchemaVersion = CurrentSchemaVersion;
    UPROPERTY(BlueprintReadWrite) FReversePlayerProfile Profile;
    UPROPERTY(BlueprintReadWrite) FDateTime SavedAtUtc;
};