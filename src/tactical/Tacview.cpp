#include "Tacview.h"

#include <imgui.h>

#include "../orbital/Components.h"
#include "../orbital/KeplerSolver.h"

namespace cosmo::tactical {

constexpr double kAU = 149597870700.0;
constexpr float kZoomMin = 0.1;
constexpr float kZoomMax = 1000.0;

constexpr int kPlanetLineSegmentCount = 200;

void Tacview::Render(entt::registry& registry,
                     sf::RenderTexture& render_texture) {
  for (const auto line : orbital_lines_) {
    render_texture.draw(line.second);
  }

  sf::CircleShape orbital_circle;
  orbital_circle.setFillColor(sf::Color::White);
  orbital_circle.setOutlineColor(sf::Color::Green);
  orbital_circle.setRadius(5);
  orbital_circle.setOutlineThickness(1.0);
  orbital_circle.setOrigin({2.5, 2.5});
  sf::Text body_label(view_params_.label_font);
  body_label.setFillColor(sf::Color::White);
  body_label.setCharacterSize(16);
  const auto view = registry.view<const orbital::components::Position,
                                  const orbital::components::Name>();
  view.each([this, &orbital_circle, &body_label, &render_texture](
                const auto& position, const auto& name) {
    const auto pos = GetScreenPosition(position);
    orbital_circle.setPosition(pos);
    body_label.setString(name.value);
    body_label.setPosition(sf::Vector2f{pos.x + 10, pos.y + 10});

    render_texture.draw(orbital_circle);
    render_texture.draw(body_label);
  });
}

void Tacview::DisplayControlUi() {
  ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
  ImGui::Begin("Tacview controls");

  if (ImGui::SliderFloat("Zoom", &view_params_.visual_scale, kZoomMin, kZoomMax)) {
    needs_update_ = true;
  }

  ImGui::End();
}

void Tacview::UpdateOrbitalLines(entt::registry& registry) {
  orbital_lines_.clear();
  const auto view = registry.view<const orbital::components::KeplerParameters,
                                  const orbital::components::Name>();

  for (const auto [_, params, name] : view.each()) {
    sf::VertexArray orbital_line(sf::PrimitiveType::LineStrip,
                                 kPlanetLineSegmentCount + 1);

    for (auto i = 0; i <= kPlanetLineSegmentCount; i++) {
      double M = (i / (double)kPlanetLineSegmentCount) * 2.0 * std::numbers::pi;
      orbital::components::Position point_pos =
          orbital::KeplerSolver::ComputePositionFromMeanAnomaly(params, M, 5);
      const auto screen_pos = GetScreenPosition(point_pos);
      orbital_line[i].position = screen_pos;
      orbital_line[i].color = sf::Color(255, 255, 255, 100);
    }

    orbital_lines_.insert({name.value, orbital_line});
  }
}

void Tacview::Update(entt::registry& registry) {
    if (needs_update_) {
        UpdateOrbitalLines(registry);
        needs_update_ = false;
    }
}

sf::Vector2f Tacview::GetScreenPosition(
    const orbital::components::Position& position) {
  const auto rel_x = position.x - view_params_.focal_point.x;
  const auto rel_y = position.y - view_params_.focal_point.y;

  const float screen_x =
      view_params_.screen_center.x +
      static_cast<float>((rel_x / kAU) * view_params_.visual_scale);
  const float screen_y =
      view_params_.screen_center.y -
      static_cast<float>((rel_y / kAU) * view_params_.visual_scale);

  return sf::Vector2f{screen_x, screen_y};
}

}  // namespace cosmo::tactical
