# Phase 5 — Game Systems

Phase 5 develops the actual game around the Phase 4 drag runtime while Unreal/Android hardware validation remains queued.

## Scope

- Persistent player profile and career progression
- Multi-car garage and vehicle ownership
- Data-driven parts catalog and inventory
- Install/remove/swap parts with compatibility rules
- Build sheets and derived performance stats
- Tuning presets and saved setups
- Race event definitions, eligibility, rewards and opponent profiles
- Economy ledger with auditable money/Street Cred transactions
- Android-first UI/view-model contracts
- Save schema versioning and migration boundaries
- Deterministic headless tests in GitHub Actions

## Design rules

1. Phase 4 physics remains authoritative for race simulation.
2. Game systems are data-driven and must not hard-code one vehicle class.
3. Android is the reference UX/performance platform; Windows shares the same game state.
4. Economy and progression must be server-authoritative-ready even while local/offline scaffolding is used.
5. Production visuals are authored assets. Generated placeholder art must never become the visual target.
6. Legacy Nitto behavior is forensic reference; the new implementation is clean modern code.

## First vertical game loop

Create profile -> acquire starter car -> garage -> buy/install part -> save tune -> enter eligible event -> race -> settle rewards -> persist progression.

## Validation boundary

Software logic can be compiled/tested remotely. Unreal UI integration, APK packaging and physical-device validation remain explicit hardware gates.