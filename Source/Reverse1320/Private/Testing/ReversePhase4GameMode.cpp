#include "Testing/ReversePhase4GameMode.h"
#include "Testing/ReversePhase4TestActor.h"
#include "Engine/World.h"

AReversePhase4GameMode::AReversePhase4GameMode()
{
    DefaultPawnClass = nullptr;
}

void AReversePhase4GameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        World->SpawnActor<AReversePhase4TestActor>(AReversePhase4TestActor::StaticClass(), FTransform::Identity);
    }
}
