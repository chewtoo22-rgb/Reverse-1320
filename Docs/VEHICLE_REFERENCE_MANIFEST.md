# Vehicle Reference Manifest

These sheets are visual references supplied for the Reverse 1320 production vehicle pipeline. They are not treated as finished game meshes.

## Current reference set

| Ref | Working identity | Primary use |
|---|---|---|
| VREF-001 | Blue sport sedan | Sedan proportions, stance, wheel fitment |
| VREF-002 | Black performance SUV | SUV mass/ride-height envelope, large-body wheel fitment |
| VREF-003 | Black widebody modern muscle coupe with cyan graphic treatment | Widebody, aero, decal/material separation |
| VREF-004 | Charcoal modern muscle sedan | Sedan aero, bronze wheel/brake package |
| VREF-005 | Black classic muscle restomod | Classic body, hood treatment, drag/restomod stance |
| VREF-006 | Orange modern performance coupe | Modern coupe stance and aero reference |
| VREF-007 | Black 1980s performance coupe | Box-body proportions and street/strip configuration |
| VREF-008 | Charcoal modern muscle sedan turntable sheet | Detailed multi-angle modeling reference |
| VREF-009 | Blue import drag coupe | Drag-specific tire, wheelie-bar/parachute configuration |
| VREF-010 | Black compact performance pickup | Truck body class and short-wheelbase handling envelope |

## Modeling requirements

Each production vehicle must be authored as a real 3D asset with consistent world scale and named component separation. At minimum:

- body shell
- front/rear glass and side glass
- four independent wheel/tire assemblies
- brake rotors/calipers
- front/rear lighting meshes or emissive masks
- interior shell visible through glass
- exhaust outlets
- optional aero/body-kit modules
- optional drag hardware: slicks, skinnies, wheelie bar, parachute, beadlocks

## Validation views

Before a vehicle is approved it must be checked against reference imagery from front, rear, left/right profile, front three-quarter, rear three-quarter and top where available. Wheelbase and overall dimensions should be normalized using authoritative dimensional data rather than perspective measurements from the sheets.

## Production-art rule

Reference-generation artifacts, labels, watermarks and synthetic inconsistencies must never propagate into the in-game model or textures. Final topology, UVs, normals, materials, decals, lighting response and LODs are authored/validated production assets.
