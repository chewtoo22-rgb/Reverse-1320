#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ReverseVehicleTypes.generated.h"

UENUM(BlueprintType)
enum class EReverseDriveLayout : uint8
{
    FWD,
    RWD,
    AWD
};

UENUM(BlueprintType)
enum class EReverseInduction : uint8
{
    NaturallyAspirated,
    Turbocharged,
    Supercharged
};

USTRUCT(BlueprintType)
struct FReverseGearboxData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<float> GearRatios { 2.66f, 1.78f, 1.30f, 1.00f, 0.74f, 0.50f };

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float FinalDrive = 3.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ShiftTimeSeconds = 0.18f;
};

USTRUCT(BlueprintType)
struct FReverseTireData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RadiusMeters = 0.335f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BaseGripCoefficient = 1.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RollingResistanceCoefficient = 0.014f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LongitudinalStiffness = 11.0f;
};

USTRUCT(BlueprintType)
struct FReverseEnginePoint
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RPM = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TorqueNm = 300.0f;
};

UCLASS(BlueprintType)
class REVERSE1320_API UReverseVehicleDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Identity")
    FName VehicleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Identity")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chassis")
    EReverseDriveLayout DriveLayout = EReverseDriveLayout::RWD;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chassis")
    float MassKg = 1600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chassis")
    float WheelbaseMeters = 2.85f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chassis")
    float CenterOfMassHeightMeters = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aerodynamics")
    float DragCoefficient = 0.34f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aerodynamics")
    float FrontalAreaSquareMeters = 2.15f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Engine")
    EReverseInduction Induction = EReverseInduction::NaturallyAspirated;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Engine")
    TArray<FReverseEnginePoint> TorqueCurve;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Engine")
    float IdleRPM = 850.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Engine")
    float RedlineRPM = 7000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Engine")
    float DrivetrainEfficiency = 0.86f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Power Adder")
    float PeakBoostPsi = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Power Adder")
    float BoostThresholdRPM = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Power Adder")
    float NitrousTorqueMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Transmission")
    FReverseGearboxData Gearbox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tires")
    FReverseTireData DriveTires;
};
