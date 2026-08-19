# Phase 4 Status

## Completed in this checkpoint

- Canonical GitHub project initialized.
- Dedicated `phase4/android-vehicle-runtime` branch created.
- Production vehicle definition supports FWD/RWD/AWD, mass, wheelbase, center of mass, aerodynamics, torque curves, induction, boost, nitrous, gearbox and drive-tire data.
- Deterministic quarter-mile runtime implemented.
- Runtime resolves engine torque -> gearing -> wheel torque -> requested tire force -> traction limit -> aero/rolling losses -> acceleration.
- Simplified longitudinal weight transfer affects driven-axle grip.
- Turbo and supercharger boost behaviors are distinct.
- Nitrous is modeled as a data-driven torque multiplier.
- Shift events impose configurable torque interruption.
- Timeslip records reaction time, 60 ft, 330 ft, 1/8 ET/MPH, 1000 ft and 1/4 ET/MPH.
- Android low/mid/high rendering tiers added without changing the art direction.
- Vehicle-reference manifest created for the first ten supplied sheets.

## Not yet claimed complete

- Runtime has not yet been compiled by UnrealBuildTool in this repository.
- No Android APK/AAB has been produced from this branch yet.
- No physical-device run has been performed.
- Torque curves, grip values and shift behavior still require calibration against representative builds.
- Burnout/tire-temperature and track-prep state are next.
- Clutch/torque-converter behavior is next.
- Wheelie behavior and wheelie-bar constraints are next.
- Production 3D vehicle meshes are not yet ingested.

## Phase 4 completion gate

Phase 4 cannot be marked PASS until an Android build compiles, installs on a physical device, runs the complete garage-to-timeslip loop and records a reproducible quarter-mile result without placeholder-only gameplay dependencies.
