#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle/ReverseRaceVehicleComponent.h"
#include "ReverseRaceInputComponent.generated.h"

UENUM(BlueprintType)
enum class EReverseRaceCommand : uint8
{
    EnterBurnout,
    FinishBurnout,
    PreStage,
    Stage,
    ArmTree,
    Launch,
    ShiftUp,
    NitrousPressed,
    NitrousReleased
};

UCLASS(ClassGroup=(Reverse1320), meta=(BlueprintSpawnableComponent))
class REVERSE1320_API UReverseRaceInputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UReverseRaceInputComponent();

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Input")
    void BindRaceVehicle(UReverseRaceVehicleComponent* InRaceVehicle);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Input")
    void SetThrottleAxis(float Value);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Input")
    void SetBurnoutSlipAxis(float Value);

    UFUNCTION(BlueprintCallable, Category="Reverse1320|Input")
    bool SendCommand(EReverseRaceCommand Command, float ReactionTimeSeconds = 0.0f);

    UFUNCTION(BlueprintPure, Category="Reverse1320|Input")
    float GetThrottleAxis() const { return ThrottleAxis; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY() TObjectPtr<UReverseRaceVehicleComponent> RaceVehicle;
    float ThrottleAxis = 0.0f;
    float BurnoutSlipAxis = 1.0f;
};
