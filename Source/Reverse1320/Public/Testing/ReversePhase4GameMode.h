#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReversePhase4GameMode.generated.h"

UCLASS()
class REVERSE1320_API AReversePhase4GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AReversePhase4GameMode();

protected:
    virtual void BeginPlay() override;
};
