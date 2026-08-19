#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

namespace reverse1320::core {

constexpr double kGravity = 9.80665;
constexpr double kAirDensity = 1.225;
constexpr double kQuarterMileMeters = 402.336;
constexpr double kMeterToMph = 2.2369362920544;

struct TireModel {
    double radius_m = 0.335;
    double grip = 1.25;
    double rolling_resistance = 0.014;
};

struct TrackModel {
    double grip_multiplier = 1.0;
};

struct VehicleModel {
    double mass_kg = 1600.0;
    double wheelbase_m = 2.85;
    double cg_height_m = 0.55;
    double drag_coefficient = 0.34;
    double frontal_area_m2 = 2.15;
    double drivetrain_efficiency = 0.86;
    double final_drive = 3.55;
    std::array<double, 6> gear_ratios{2.66, 1.78, 1.30, 1.00, 0.74, 0.50};
    std::array<double, 8> torque_nm{420, 500, 560, 590, 600, 585, 545, 470};
    std::array<double, 8> torque_rpm{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};
    double idle_rpm = 900.0;
    double redline_rpm = 7800.0;
    double shift_time_s = 0.16;
    double nitrous_multiplier = 1.0;
    TireModel tire{};
};

struct RunResult {
    double et_s = 0.0;
    double trap_mph = 0.0;
    double sixty_s = 0.0;
    bool finished = false;
};

inline double sample_torque(const VehicleModel& v, double rpm) {
    if (rpm <= v.torque_rpm.front()) return v.torque_nm.front();
    for (std::size_t i = 1; i < v.torque_rpm.size(); ++i) {
        if (rpm <= v.torque_rpm[i]) {
            const double a = (rpm - v.torque_rpm[i - 1]) /
                std::max(1.0, v.torque_rpm[i] - v.torque_rpm[i - 1]);
            return v.torque_nm[i - 1] + (v.torque_nm[i] - v.torque_nm[i - 1]) * a;
        }
    }
    return v.torque_nm.back();
}

inline RunResult simulate_quarter_mile(const VehicleModel& v, const TrackModel& track,
                                       bool nitrous = false, double dt = 1.0 / 240.0) {
    RunResult out{};
    double t = 0.0;
    double x = 0.0;
    double speed = 0.0;
    int gear = 0;
    double shift_timer = 0.0;
    double rpm = v.idle_rpm;
    double prev_x = 0.0;

    for (int step = 0; step < 240 * 30; ++step) {
        prev_x = x;
        const double ratio = v.gear_ratios[std::clamp(gear, 0, 5)];
        const double wheel_rad_s = speed / std::max(0.05, v.tire.radius_m);
        rpm = std::max(v.idle_rpm, wheel_rad_s * ratio * v.final_drive * 60.0 / (2.0 * M_PI));

        if (rpm >= v.redline_rpm && gear < 5 && shift_timer <= 0.0) {
            ++gear;
            shift_timer = v.shift_time_s;
        }

        const double active_ratio = v.gear_ratios[std::clamp(gear, 0, 5)];
        double torque = sample_torque(v, rpm);
        if (nitrous) torque *= std::max(1.0, v.nitrous_multiplier);
        if (shift_timer > 0.0) {
            torque *= 0.12;
            shift_timer = std::max(0.0, shift_timer - dt);
        }

        const double wheel_torque = torque * active_ratio * v.final_drive * v.drivetrain_efficiency;
        const double requested_force = wheel_torque / std::max(0.05, v.tire.radius_m);
        const double static_load = v.mass_kg * kGravity;
        const double accel_guess = requested_force / std::max(1.0, v.mass_kg);
        const double transfer = v.mass_kg * accel_guess * v.cg_height_m / std::max(0.5, v.wheelbase_m);
        const double rear_load = std::max(0.0, static_load * 0.50 + transfer);
        const double tire_limit = rear_load * v.tire.grip * track.grip_multiplier;
        const double drive_force = std::min(requested_force, tire_limit);

        const double aero = 0.5 * kAirDensity * v.drag_coefficient * v.frontal_area_m2 * speed * speed;
        const double rolling = v.tire.rolling_resistance * v.mass_kg * kGravity;
        const double accel = (drive_force - aero - rolling) / std::max(1.0, v.mass_kg);

        speed = std::max(0.0, speed + accel * dt);
        x += speed * dt;
        t += dt;

        if (out.sixty_s <= 0.0 && prev_x < 18.288 && x >= 18.288) out.sixty_s = t;
        if (x >= kQuarterMileMeters) {
            out.et_s = t;
            out.trap_mph = speed * kMeterToMph;
            out.finished = true;
            return out;
        }
    }
    return out;
}

} // namespace reverse1320::core
