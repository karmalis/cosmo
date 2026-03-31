//
// Created by voltairepunk on 3/15/26.
//

#include "KeplerSolver.h"
#include <cmath>
#include <numbers>

namespace cosmo::orbital {

components::Position KeplerSolver::ComputePosition(
    const components::KeplerParameters& orbit, const double& time,
    const size_t& iterations) {
  const auto n = std::sqrt(orbit.mu / std::pow(orbit.semi_major_axis, 3));
  auto M = orbit.mean_anomaly_at_epoch + n * time;
  return ComputePositionFromMeanAnomaly(orbit, M, iterations);
}

components::Position KeplerSolver::ComputePositionFromMeanAnomaly(
    const components::KeplerParameters& orbit, const double& mean_anomaly,
    const size_t& iterations) {
  // Wrap M to [0, 2pi]
  auto M = std::fmod(mean_anomaly, 2.0 * std::numbers::pi);
  if (M < 0) M += 2.0 * std::numbers::pi;

  auto E = M;
  for (auto i = 0; i < iterations; ++i) {
    const auto delta_e = (E - orbit.eccentricity * std::sin(E) - M) /
                         (1.0 - orbit.eccentricity * std::cos(E));
    E -= delta_e;
    if (std::abs(delta_e) < 1e-6) break;
  }

  const auto cos_E = std::cos(E);
  const auto sin_E = std::sin(E);

  // Position in orbital plane (x points toward periapsis)
  const auto x_orb = orbit.semi_major_axis * (cos_E - orbit.eccentricity);
  const auto y_orb = orbit.semi_major_axis *
                     std::sqrt(1.0 - std::pow(orbit.eccentricity, 2)) * sin_E;

  // Rotation angles in radians
  const auto i = orbit.inclination;
  const auto Om = orbit.longitude_of_asc_node;
  const auto om = orbit.argument_of_periapsis;

  const auto cos_om = std::cos(om);
  const auto sin_om = std::sin(om);
  const auto cos_Om = std::cos(Om);
  const auto sin_Om = std::sin(Om);
  const auto cos_i = std::cos(i);
  const auto sin_i = std::sin(i);

  // Transform from orbital plane to reference frame (3D rotation)
  const auto x =
      static_cast<double>(x_orb * (cos_om * cos_Om - sin_om * sin_Om * cos_i) -
                          y_orb * (sin_om * cos_Om + cos_om * sin_Om * cos_i));
  const auto y =
      static_cast<double>(x_orb * (cos_om * sin_Om + sin_om * cos_Om * cos_i) +
                          y_orb * (cos_om * cos_Om * cos_i - sin_om * sin_Om));
  const auto z =
      static_cast<double>(x_orb * (sin_om * sin_i) + y_orb * (cos_om * sin_i));

  return {x, y, z};
}

}  // namespace cosmo::orbital
