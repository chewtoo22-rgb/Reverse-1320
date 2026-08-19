#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vehicle/ReverseRaceVehicleComponent.h"
#include "ReversePhase4TestActor.generated.h"

UCLASS()
class REVERSE1320_API AReversePhase4TestActor : public AActor
{
    GENERATED_BODY()

public:
    AReversePhase4TestActor();

    UFUNCTION(BlueprintPure, Category="Reverse1320|Testing")
    FString GetStatusText() const;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UReverseDragPhysicsComponent> DragPhysics;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UReverseLaunchDynamicsComponent> LaunchDynamics;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UReverseRaceVehicleComponent> RaceVehicle;

    UPROPERTY(Transient)
    TObjectPtr<UReverseVehicleDefinition> TestVehicle;

    float PhaseClock = 0.0f;
    int32 LastShiftedGear = 1;
    bool bReportedFinish = false;

    void BuildTransientTestVehicle();
    void RunAutomatedSequence(float DeltaSeconds);
};
