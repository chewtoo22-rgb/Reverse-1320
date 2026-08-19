# Phase 4 — Android Vehicle Runtime

## Goal

Turn the Phase 3 gameplay architecture into the first real Android-driving implementation while keeping the codebase Windows-capable.

## Hard gates

1. Production vehicle data model supports FWD, RWD and AWD.
2. Engine model supports naturally aspirated, turbo, supercharger and nitrous-assisted combinations.
3. Launch model includes tire grip, wheelspin, weight transfer and launch-control behavior.
4. Transmission model includes per-gear ratios, final drive, shift duration and rev drop.
5. Quarter-mile runtime records RT, 60 ft, 330 ft, 1/8 ET/MPH, 1000 ft and 1/4 ET/MPH.
6. Android graphics tiers reduce rendering cost without replacing production art with lower-quality style assets.
7. Touch and controller inputs share the same gameplay command layer.
8. Vehicle reference sheets remain reference material; final game vehicles are proper 3D meshes with authored PBR materials and LODs.
9. The first physical Android package/install is a mandatory Phase 4 completion gate.

## Phase 4 vertical slice

Garage -> select vehicle -> inspect build -> install parts -> tune -> burnout/stage -> tree -> launch -> shift -> nitrous -> finish -> timeslip -> save.

## Physics direction

The runtime is deterministic enough for multiplayer validation but intentionally data-driven. No single magic horsepower-to-ET formula is used. Acceleration is resolved from wheel force, grip, aero, rolling resistance, drivetrain loss and mass at each simulation step.

## Visual direction

The uploaded vehicle sheets establish proportion, stance and customization references. In-game vehicles must exceed the sheets in lighting/material fidelity. Required production features include separated wheel/tire assemblies, brake components, glass, emissive lighting, paint layers, carbon/metal materials, suspension travel and optional drag hardware.
