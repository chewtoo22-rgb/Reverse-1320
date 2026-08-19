#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle/ReverseDragPhysicsComponent.h"
#include "Vehicle/ReverseLaunchDynamicsComponent.h"
#include "ReverseRaceVehicleComponent.generated.h"

UENUM(BlueprintType)
enum class EReverseRaceVehiclePhase : uint8
{
    Garage,
    Burnout,
    PreStage,
    Staged,
    Tree,
    Racing,
    Finished
};

UCLASS(ClassGroup=(Reverse1320), meta=(BlueprintSpawnableComponent))
class REVERSE1320_API UReverseRaceVehicleComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UReverseRaceVehicleComponent();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void ConfigureVehicle(UReverseVehicleDefinition* InDefinition, const FReverseTrackSurfaceData& InTrack);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void EnterBurnout();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void SetBurnoutInput(float Throttle, float WheelSlipRatio);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void FinishBurnout();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void PreStage();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void Stage();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void ArmTree();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    bool Launch(float ReactionTimeSeconds, float Throttle = 1.0f);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    bool ShiftUp();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void SetNitrous(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|RaceVehicle")
    void SetThrottle(float Value);

    UFUNCTION(BlueprintPure, Category="Reverse1320|RaceVehicle")
    EReverseRaceVehiclePhase GetPhase() const { return Phase; }

    UFUNCTION(BlueprintPure, Category="Reverse1320|RaceVehicle")
    FReverseTimingSlip GetTimingSlip() const;

    UFUNCTION(BlueprintPure, Category="Reverse1320|RaceVehicle")
    FReverseLaunchRuntimeState GetLaunchState() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY() TObjectPtr<UReverseVehicleDefinition> VehicleDefinition;
    UPROPERTY() TObjectPtr<UReverseDragPhysicsComponent> DragPhysics;
    UPROPERTY() TObjectPtr<UReverseLaunchDynamicsComponent> LaunchDynamics;

    FReverseTrackSurfaceData TrackSurface;
    EReverseRaceVehiclePhase Phase = EReverseRaceVehiclePhase::Garage;
    float BurnoutThrottle = 0.0f;
    float BurnoutSlip = 0.0f;
};
