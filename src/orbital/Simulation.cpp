//
// Created by voltairepunk on 3/15/26.
//

#include "Simulation.h"

#include "BodyRenderer.h"
#include "Components.h"

namespace cosmo::orbital {

Simulation::Simulation() = default;

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
  accumulator_ += real_delta_time * time_compression_;

  while (accumulator_ >= kTimeStep) {
    SingleSimulationStep(registry, kTimeStep);
    simulation_time_ += kTimeStep;
    accumulator_ -= kTimeStep;
  }
}
void Simulation::SingleSimulationStep(entt::registry& registry,
                                      const double& time_step) {
  const auto view =
      registry.view<components::Position, const components::KeplerParameters>();

  view.each([this](auto& position, const auto& params) {
    position = solver_.ComputePosition(params, simulation_time_, 10);
  });
}
void Simulation::Render(entt::registry& registry,
                        sf::RenderTexture& render_texture, sf::Font& font) {
  const auto view =
      registry.view<const components::Position, const components::Name>();

  // 2560, 1440u
  float cx = 2560.f / 2.f;
  float cy = 1440.f / 2.f;

  BodyRenderer::RenderParams body_render_params {
    .focal_point = components::Position{.x = 0.0, .y = 0.0, .z = 0.0},
    .screen_center = sf::Vector2f{cx, cy},
    .visual_scale = 10.0,
    .font = font
  };

  view.each(
      [this, &render_texture, &body_render_params](const auto& position, const auto& name) {
        BodyRenderer::Render(name.value, render_texture, position, body_render_params);
      });
}
}  // namespace cosmo::orbital
