//
// Created by voltairepunk on 3/15/26.
//

#include "Simulation.h"

#include <imgui.h>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "Components.h"
#include "KeplerSolver.h"

namespace cosmo::orbital {

Simulation::Simulation() {}

bool Simulation::Initialize(entt::registry& registry,
                            const std::string& system_path) {
  if (const auto definition = SystemFactory::LoadDefinition(system_path);
      definition.has_value()) {
    SystemFactory::SpawnStar(registry, definition.value());
    SystemFactory::SpawnSystem(registry, definition.value());

    return true;
  }

  return false;
}

void Simulation::Update(entt::registry& registry, const sf::Time& dt) {
  const auto real_delta_time = static_cast<double>(dt.asSeconds());
  simulation_time_ += real_delta_time * time_compression_;

  // For analytical orbits, we can update positions directly to the new
  // simulation_time_ without fixed-step integration.
  const auto view =
      registry.view<components::Position, const components::KeplerParameters>();

  view.each([this](auto& position, const auto& params) {
    position = solver_.ComputePosition(params, simulation_time_, 5);
  });

  // if (should_compute_orbits) {

  //     orbit_lines_.emplace(name, ComputeOrbitLine(params));
  //   }

  // If we had numerical components, we would still use the accumulator here
  // accumulator_ += real_delta_time * time_compression_;
  // while (accumulator_ >= kTimeStep) {
  //   ...
  // }
}

void Simulation::SingleSimulationStep(entt::registry& registry,
                                      const double& time_step) {
  // Currently unused as orbits are analytical, but kept for future numerical
  // integration.
}

void Simulation::DisplayControlUi() {
  ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
  ImGui::Begin("Simulation controls");

  ImGui::Text("Simulation Time: %.2f s", simulation_time_);

  // Use a logarithmic scale for time compression as it can span many orders
  // of magnitude
  static float log_tc = std::log10(static_cast<float>(time_compression_));
  if (ImGui::SliderFloat("Time Compression", &log_tc, 0.0f, 10.0f)) {
    time_compression_ = std::pow(10.0, static_cast<double>(log_tc));
  }
  ImGui::Text("Current Speed: %.0fx", time_compression_);

  if (ImGui::Button("Reset Time")) {
    simulation_time_ = 0.0;
  }
  ImGui::SameLine();
  if (ImGui::Button("Pause")) {
    time_compression_ = 0.0;
    log_tc = 0.0f;
  }

  ImGui::End();
}

}  // namespace cosmo::orbital
