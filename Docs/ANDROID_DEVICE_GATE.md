# Phase 4 Android Device Gate

Phase 4 is complete only when the Android build is demonstrated on physical hardware. Source completeness alone is not enough.

## Runner requirements

The `Phase 4 Unreal Android` workflow expects a self-hosted Windows runner with labels:

- `self-hosted`
- `windows`
- `unreal-5.8`
- `android`

The runner must provide:

- Unreal Engine 5.8 with Android platform support
- Android SDK/NDK/JDK versions accepted by that Unreal installation
- `UE_ROOT` environment variable pointing at the Unreal installation root
- enough disk space for cook/package output

## Package gate

The workflow must produce a Development Android package artifact from `Reverse1320.uproject` with no hand-edited generated files.

## Physical-device gate

Install the produced package on a supported ARM64 Android device and verify, in this order:

1. App launches without crash.
2. Garage/profile state initializes.
3. A vehicle definition loads.
4. Touch throttle responds continuously from 0-100%.
5. Controller and touch commands hit the same command layer.
6. Burnout raises tire temperature and changes available launch grip.
7. Pre-stage and stage states transition correctly.
8. Tree/launch records reaction time.
9. Manual shift interrupts torque for the configured shift duration.
10. Nitrous changes acceleration only when equipped/enabled.
11. Quarter-mile run captures 60 ft, 330 ft, 1/8 ET/MPH, 1000 ft and 1/4 ET/MPH.
12. Timeslip remains deterministic within an agreed tolerance when the same inputs/configuration are replayed.
13. Save/profile state survives process restart.
14. No placeholder-only dependency blocks completing the loop.

## Performance gate

On the Android reference device:

- physics simulation must remain stable under frame-rate variation
- input must remain responsive during burnout, launch and shifts
- thermal throttling must not corrupt timing or state transitions
- Low/Mid/High visual profiles may reduce cost, never replace production-quality assets with alternate low-quality art

## Production vehicle gate

At least one approved 3D vehicle must be ingested with correct scale, wheelbase, wheel/tire separation, brakes, glass, lighting, interior shell, authored materials and Android LODs. Reference sheets alone do not satisfy this gate.
