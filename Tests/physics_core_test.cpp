#include "../Tools/PhysicsCore/reverse1320_core.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace reverse1320::core;

int main() {
    VehicleModel street{};
    TrackModel good_track{1.05};
    TrackModel poor_track{0.72};

    const auto baseline = simulate_quarter_mile(street, good_track, false);
    assert(baseline.finished);
    assert(baseline.et_s > 8.0 && baseline.et_s < 20.0);
    assert(baseline.trap_mph > 70.0 && baseline.trap_mph < 220.0);
    assert(baseline.sixty_s > 0.8 && baseline.sixty_s < 4.0);

    const auto poor = simulate_quarter_mile(street, poor_track, false);
    assert(poor.finished);
    assert(poor.et_s > baseline.et_s);

    VehicleModel nitrous_car = street;
    nitrous_car.nitrous_multiplier = 1.22;
    const auto spray = simulate_quarter_mile(nitrous_car, good_track, true);
    assert(spray.finished);
    assert(spray.et_s < baseline.et_s);
    assert(spray.trap_mph > baseline.trap_mph);

    VehicleModel heavy = street;
    heavy.mass_kg += 500.0;
    const auto heavy_run = simulate_quarter_mile(heavy, good_track, false);
    assert(heavy_run.finished);
    assert(heavy_run.et_s > baseline.et_s);

    std::cout << "compiled physics core: PASS\n";
    std::cout << "baseline_et=" << baseline.et_s << " trap=" << baseline.trap_mph << " sixty=" << baseline.sixty_s << "\n";
    std::cout << "poor_track_et=" << poor.et_s << "\n";
    std::cout << "nitrous_et=" << spray.et_s << " trap=" << spray.trap_mph << "\n";
    std::cout << "heavy_et=" << heavy_run.et_s << "\n";
    return 0;
}
