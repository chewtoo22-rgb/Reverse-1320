#!/usr/bin/env python3
"""Fast deterministic sanity checks for Reverse 1320 Phase 4 physics assumptions.

This does not replace UnrealBuildTool or device validation. It protects the data-model
invariants that should never regress while the real UE runtime is being integrated.
"""

from dataclasses import dataclass


@dataclass
class TireThermals:
    ambient: float = 24.0
    optimal: float = 82.0
    maximum_useful: float = 118.0
    cold_grip: float = 0.72
    hot_grip: float = 1.08


@dataclass
class Track:
    base_grip: float = 1.0
    prep: float = 0.70
    rubber: float = 0.55
    surface_temp_c: float = 32.0
    moisture: float = 0.0
    degradation: float = 0.0


def clamp(x, lo, hi):
    return max(lo, min(hi, x))


def lerp(a, b, t):
    return a + (b - a) * t


def tire_grip(temp_c: float, t: TireThermals) -> float:
    if temp_c <= t.optimal:
        alpha = clamp((temp_c - t.ambient) / max(1.0, t.optimal - t.ambient), 0.0, 1.0)
        return lerp(t.cold_grip, t.hot_grip, alpha)
    overheat = clamp((temp_c - t.optimal) / max(1.0, t.maximum_useful - t.optimal), 0.0, 1.0)
    return lerp(t.hot_grip, 0.78, overheat)


def surface_grip(track: Track) -> float:
    prep = clamp(track.prep, 0.0, 1.0)
    rubber = clamp(track.rubber, 0.0, 1.0)
    moisture_penalty = lerp(1.0, 0.42, clamp(track.moisture, 0.0, 1.0))
    degradation_penalty = lerp(1.0, 0.80, clamp(track.degradation, 0.0, 1.0))
    temp_bonus = clamp(1.0 + (track.surface_temp_c - 25.0) * 0.0025, 0.92, 1.08)
    return (
        track.base_grip
        * lerp(0.82, 1.12, prep)
        * lerp(0.92, 1.06, rubber)
        * moisture_penalty
        * degradation_penalty
        * temp_bonus
    )


def main() -> None:
    thermals = TireThermals()
    cold = tire_grip(thermals.ambient, thermals)
    warm = tire_grip(60.0, thermals)
    optimal = tire_grip(thermals.optimal, thermals)
    overheated = tire_grip(thermals.maximum_useful, thermals)

    assert cold < warm < optimal, (cold, warm, optimal)
    assert overheated < optimal, (overheated, optimal)
    assert abs(optimal - thermals.hot_grip) < 1e-6

    dry_prepped = surface_grip(Track(prep=0.95, rubber=0.85, moisture=0.0))
    dry_poor = surface_grip(Track(prep=0.10, rubber=0.10, moisture=0.0))
    wet_prepped = surface_grip(Track(prep=0.95, rubber=0.85, moisture=0.75))

    assert dry_prepped > dry_poor, (dry_prepped, dry_poor)
    assert wet_prepped < dry_prepped, (wet_prepped, dry_prepped)

    # Overheating must never become a hidden traction upgrade.
    combined_optimal = dry_prepped * optimal
    combined_overheated = dry_prepped * overheated
    assert combined_overheated < combined_optimal

    print("Phase 4 physics sanity: PASS")
    print(f"cold_tire_grip={cold:.3f}")
    print(f"optimal_tire_grip={optimal:.3f}")
    print(f"overheated_tire_grip={overheated:.3f}")
    print(f"prepped_track={dry_prepped:.3f}")
    print(f"poor_track={dry_poor:.3f}")
    print(f"wet_prepped_track={wet_prepped:.3f}")


if __name__ == "__main__":
    main()
