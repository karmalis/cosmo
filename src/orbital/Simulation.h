//
// Created by voltairepunk on 3/15/26.
//

#ifndef COSMO_SYSTEM_H
#define COSMO_SYSTEM_H

#include <SFML/Graphics/VertexArray.hpp>
#include <entt/entt.hpp>

#include "KeplerSolver.h"
#include "SystemFactory.h"
#include <SFML/Graphics.hpp>

#include <unordered_map>

namespace cosmo::orbital {

constexpr double kTimeStep = 1;
constexpr double kDefaultTimeCompression = 1000.0;

class Simulation {
 public:
  Simulation();

  bool Initialize(entt::registry& registry, const std::string& system_path);
  void Update(entt::registry& registry, const sf::Time& dt);

  void SingleSimulationStep(entt::registry& registry, const double& time_step);

  void DisplayControlUi();

 private:
  double simulation_time_{};
  double time_compression_{kDefaultTimeCompression};
  double accumulator_{};

  KeplerSolver solver_;
};

}  // namespace cosmo::orbital

#endif  // COSMO_SYSTEM_H
