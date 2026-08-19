# Phase 4 Status

## Completed in this checkpoint

- Canonical GitHub project initialized.
- Dedicated `phase4/android-vehicle-runtime` branch created.
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
- GitHub Actions physics-sanity workflow added for tire/track invariants; CI is active on the Phase 4 branch/PR.

## Not yet claimed complete

- Runtime has not yet been compiled by UnrealBuildTool in this repository.
- No Android APK/AAB has been produced from this branch yet.
- No physical-device run has been performed.
- Torque curves, grip values, converter behavior and shift behavior still require calibration against representative builds.
- Production 3D vehicle meshes are not yet ingested.
- Burnout smoke, tire deformation, suspension/wheelie animation and track VFX are not yet implemented as production visuals.

## Phase 4 completion gate

Phase 4 cannot be marked PASS until an Android build compiles, installs on a physical device, runs the complete garage-to-timeslip loop and records a reproducible quarter-mile result without placeholder-only gameplay dependencies.
