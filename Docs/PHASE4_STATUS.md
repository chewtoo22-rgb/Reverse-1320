# Phase 4 Status

## Completed in this checkpoint

- Canonical GitHub project initialized.
- Dedicated `phase4/android-vehicle-runtime` branch created.
- Project is now a real Unreal project with `.uproject`, runtime module, game/editor targets and Android configuration.
- Production vehicle definition supports FWD/RWD/AWD, mass, wheelbase, center of mass, aerodynamics, torque curves, induction, boost, nitrous, gearbox and drive-tire data.
- Deterministic quarter-mile runtime implemented.
- Runtime resolves engine torque -> gearing -> wheel torque -> requested tire force -> traction limit -> aero/rolling losses -> acceleration.
- Longitudinal weight transfer affects driven-axle grip.
- Turbo and supercharger boost behaviors are distinct.
- Nitrous is modeled as a data-driven torque multiplier.
- Shift events impose configurable torque interruption.
- Timeslip records reaction time, 60 ft, 330 ft, 1/8 ET/MPH, 1000 ft and 1/4 ET/MPH.
- Tire thermal model supports cold, optimal and overheated grip states.
- Burnout input heats the drive tires; coasting/staging cools them toward ambient.
- Track grip responds to prep level, rubber level, surface temperature, moisture and lane degradation.
- Direct clutch, torque-converter and dual-clutch launch device types are represented.
- Torque-converter launch multiplication depends on stall-RPM proximity and ramps into the drag solver.
- Wheelie demand is estimated from longitudinal load transfer and center-of-mass geometry.
- Optional wheelie-bar contact caps pitch and prevents complete front-axle unloading.
- Launch dynamics feed dynamic grip and launch-torque multipliers directly into the quarter-mile runtime.
- Race vehicle coordinator owns burnout -> pre-stage -> stage -> tree -> launch -> shift/nitrous -> finish.
- Shared race input component gives touch UI and gamepad/controller one command path for throttle, staging, launch, shifting and nitrous.
- Android low/mid/high rendering tiers are present without changing the production-art direction.
- Vehicle-reference manifest tracks the first ten supplied reference sheets.
- Representative vehicle calibration templates added for import-drag, muscle-sedan, performance-SUV and compact-pickup classes; these are explicit calibration seeds rather than claimed OEM specifications.
- GitHub Actions tire/track invariant workflow is green.
- Standalone C++17 deterministic drag core compiles and runs in GitHub Actions.
- Compiled-core regression output: baseline 11.5208 @ 129.96 mph, poor-track 12.5958, nitrous 10.8333 @ 137.332 mph, +500 kg 12.8417.
- Unreal Android package workflow added for a self-hosted Windows runner with UE 5.8 + Android toolchain.
- Android physical-device gate documented explicitly.
- Package boot path now uses a C++ Phase 4 game mode and automated vertical-slice actor requiring no project-authored binary assets.
- Automated package test performs burnout -> pre-stage -> stage -> tree -> launch -> automatic shifts/nitrous -> finish and emits live telemetry plus a full timeslip to the screen/log.

## Remaining hard boundaries

- UnrealBuildTool has not yet compiled this branch on an Unreal-capable runner.
- No Android APK/AAB has yet been produced from this branch.
- No physical-device run has yet been performed.
- Production 3D vehicle meshes are not yet ingested; reference sheets are not considered game-ready meshes.
- Torque curves, grip values, converter behavior and shift behavior still require calibration against representative real builds and desired game balance.
- Burnout smoke, tire deformation, suspension/wheelie animation and track VFX are not yet implemented as production visuals.

## Phase 4 completion gate

Phase 4 cannot be marked PASS until an Unreal 5.8 Android build compiles, packages, installs on physical ARM64 Android hardware, runs the complete device test/garage-to-timeslip path, records a reproducible quarter-mile result, and at least one approved production 3D vehicle is ingested with authored materials/LODs rather than placeholder-only art.
